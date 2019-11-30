#ifndef TOBJECT_POOL_HXX
#define TOBJECT_POOL_HXX

#include <array>
#include <list>

template <typename T, int size>

class TObjectPool
{
   struct Object
   {
      Object* next;
      bool inuse;
      T element;
   };
   
   struct Block
   {
      std::array<Object, size> objects;
   };
   
   std::list<Block*> blocks;
   Object* freelist;

   void allocate_block()
   {
      Block* block = new Block;
      blocks.push_front( block );

      for ( auto& object : block->objects )
      {
	 object.next = freelist;
         object.inuse = false;
	 freelist = &object;
         count += 1;
         free += 1;
      }
   }
   
public:
   TObjectPool() : freelist(nullptr), count(0), free(0)
   {
      allocate_block();
   }

   unsigned count;
   unsigned free;

   T* get( bool force = false )
   {
      if ( freelist == nullptr )
      {
	 if ( !force )
            return nullptr;
         
         allocate_block();
      }

      Object* object = freelist;
      freelist = freelist->next;

      object->inuse = true;
      free -= 1;
      
      return &object->element;
   }

   void sweep()
   {
      freelist = nullptr;
      free = 0;

      for ( auto block : blocks )
      {
	 for ( auto& object : block->objects )
	 {
	    if ( object.element.ismarked() )
	    {
	       object.element.unmark();
	    }
	    else
	    {
               // desconstruct using the virtual destructor
               if ( object.inuse )
               {
                  object.inuse = false;
                  object.element.~T();
               }
	       object.next = freelist;
	       freelist = &object;

               free += 1;
	    }
	 }
      }

      // printf( "size=%d, free=%d, size/4=%d, trigger=%d\n", size, free, size/4, (free < size/4) );

      if ( free < size / 4 )
      {
         allocate_block();
      }
   }
};

#endif

