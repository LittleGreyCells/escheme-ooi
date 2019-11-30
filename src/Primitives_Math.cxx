#include "Primitives_Math.hxx"

#include <math.h>

#include "Memory.hxx"
#include "Symbol.hxx"
#include "argstack.hxx"

namespace scheme
{
   namespace Primitives
   {
      enum BinOp { ADDop, SUBop, MULop, DIVop, 
                   QUOop, REMop, 
                   MINop, MAXop,
                   ANDop, IORop, XORop };
         
      enum RelOp { EQop, LTop, LEop, GTop, GEop };
         
      using FIXNUM = long int;
      using UFIXNUM = unsigned long int;
      using FLONUM = double;
         
      static void badoperand() { throw SevereException("bad operand type"); }
      static void badoperator() { throw SevereException("bad operator"); }
         
      static void ZeroTest( FIXNUM a ) { if (a == 0) throw SevereException("division by zero"); }
      static void ZeroTest( FLONUM a ) { if (a == 0.0) throw SevereException("division by zero"); }
         
      static Node* binary( BinOp op )
      {
         ArgstackIterator iter;
         Node* a = guard(iter.getarg(), &Node::numberp);
            
         FIXNUM x=0, y=0;
         FLONUM fx=0, fy=0;
         bool flop;
            
         if ( a->fixnump() )
         {
            x = a->getfixnum();
            flop = false;
         }
         else
         {
            fx = a->getflonum();
            flop = true;
         }
            
         // single operand?
            
         if ( !iter.more() )
         {
            switch ( op )
            {
               case SUBop: 
                  if ( flop ) 
                     fx = -fx; 
                  else 
                     x = -x; 
                  break;
                     
               case DIVop: 
                  if ( flop )
                  {
                     ZeroTest(fx);
                     fx = 1.0 / fx;
                  }
                  else
                  {
                     ZeroTest(x);
                     fx = 1.0 / static_cast<FLONUM>(x);
                     flop = true;
                  }
                  break;
                     
               default:
                  break;
            }
         }
            
         // many operands
            
         while ( iter.more() )
         {
            a = guard(iter.getarg(), &Node::numberp);
               
            if ( flop )
            {
               if ( a->fixnump() )
                  fy = static_cast<FLONUM>( a->getfixnum() );
               else
                  fy = a->getflonum();
            }
            else
            {
               if ( a->fixnump() )
               {
                  y = a->getfixnum();
               }
               else
               {
                  fx = static_cast<FLONUM>(x);
                  fy = a->getflonum();
                  flop = true;
               }
            }
               
            switch (op)
            {
               case MINop: if ( flop ) fx = std::min(fx, fy); else x = std::min(x, y); break;
               case MAXop: if ( flop ) fx = std::max(fx, fy); else x = std::max(x, y); break;
               case ANDop: if ( flop ) badoperand(); else x &= y; break;
               case IORop: if ( flop ) badoperand(); else x |= y; break;
               case XORop: if ( flop ) badoperand(); else x ^= y; break;
               case ADDop: if ( flop ) fx += fy; else x += y; break;
               case SUBop: if ( flop ) fx -= fy; else x -= y; break;
               case MULop: if ( flop ) fx *= fy; else x *= y; break;
               case QUOop: if ( flop ) { badoperand(); } else { ZeroTest(y); x /= y; } break;
               case REMop: if ( flop ) { badoperand(); } else { ZeroTest(y); x %= y; } break;
               case DIVop: if ( flop ) 
               { 
                  ZeroTest(fy); 
                  fx /= fy; 
               } 
               else
               { 
                  ZeroTest(y);
                  if ( x % y == 0 )
                  {
                     x /= y;
                  }
                  else
                  {
                     fx = static_cast<FLONUM>(x);
                     fy = static_cast<FLONUM>(y);
                     fx /= fy;
                     flop = true;
                  }
               } 
                  break;
               default:
                  badoperator();
            }
         }
            
         if ( flop )
            return Memory::flonum(fx);
         else
            return Memory::fixnum(x);
      }
         
      static Node* relation( RelOp op )
      {
         ArgstackIterator iter;
         Node* a = guard(iter.getarg(), &Node::numberp);
            
         FIXNUM x=0, y=0;
         FLONUM fx=0, fy=0;
         int r = 1;
         bool flop;

         if ( a->fixnump() )
         {
            x = a->getfixnum();
            flop = false;
         }
         else
         {
            fx = a->getflonum();
            flop = true;
         }

         while ( iter.more() )
         {
            a = guard(iter.getarg(), &Node::numberp);

            if ( flop )
            {
               if ( a->fixnump() )
                  fy = static_cast<FLONUM>(a->getfixnum());
               else
                  fy = a->getflonum();
            }
            else
            {
               if ( a->fixnump() )
               {
                  y = a->getfixnum();
               }
               else
               {
                  fx = static_cast<FLONUM>(x);
                  fy = a->getflonum();
                  flop = true;
               }
            }

            switch (op)
            {
               case EQop: r = flop ? (fx == fy) : (x == y); break;
               case LTop: r = flop ? (fx <  fy) : (x <  y); break;
               case LEop: r = flop ? (fx <= fy) : (x <= y); break;
               case GTop: r = flop ? (fx >  fy) : (x >  y); break;
               case GEop: r = flop ? (fx >= fy) : (x >= y); break;
               default:
                  badoperator();
            }

            if ( r == 0 )
               return symbol_false;

            if ( flop )
               fx = fy;
            else 
               x = y;
         }

         return r ? symbol_true : symbol_false;
      }

      Node* add() 
      { 
         ArgstackIterator iter;
         return ( !iter.more() ) ? Memory::fixnum(0) : binary(ADDop);
      }

      Node* mul()
      { 
         ArgstackIterator iter;
         return ( !iter.more() ) ? Memory::fixnum(1) : binary(MULop);
      }

      Node* sub() { return binary(SUBop); }
      Node* div() { return binary(DIVop); }

      Node* min() { return binary(MINop); }
      Node* max() { return binary(MAXop); }

      Node* logand() { return binary(ANDop); }
      Node* logior() { return binary(IORop); }
      Node* logxor() { return binary(XORop); }

      Node* quotient() { return binary(QUOop); }
      Node* remainder() { return binary(REMop); }

      namespace relop
      {
         Node* eq() { return relation(EQop); }
         Node* lt() { return relation(LTop); }
         Node* le() { return relation(LEop); }
         Node* gt() { return relation(GTop); }
         Node* ge() { return relation(GEop); }
      }
         
      Node* truncate()
      {
         ArgstackIterator iter;
         Node* a = guard(iter.getlast(), &Node::numberp);
         return a->fixnump() ? a : Memory::fixnum(static_cast<FIXNUM>(a->getflonum()));
      }

      Node* floor()
      {
         ArgstackIterator iter;
         Node* a = guard(iter.getlast(), &Node::numberp);
         if ( a->fixnump() )
         {
            return a;
         }
         else
         {
            const FLONUM n = a->getflonum();
            const FIXNUM fixn = static_cast<FIXNUM>(n);
            return Memory::fixnum( (n > 0.0) ? fixn : fixn - 1 );
         }
      }

      Node* ceiling()
      {
         ArgstackIterator iter;
         Node* a = guard(iter.getlast(), &Node::numberp);
         if ( a->fixnump() )
         {
            return a;
         }
         else
         {
            const FLONUM n = a->getflonum();
            const FIXNUM fixn = static_cast<FIXNUM>(n);
            return Memory::fixnum( (n > 0.0) ? fixn + 1 : fixn );
         }
      }

      Node* round()
      {
         ArgstackIterator iter;
         Node* a = guard(iter.getlast(), &Node::numberp);
         if ( a->fixnump() )
            return a;
         else
         {
            const FLONUM n = a->getflonum();
            return Memory::fixnum( static_cast<FIXNUM>(n + ((n < 0.0) ? -0.5 : +0.5)) );
         }
      }

      Node* inc()
      {
         ArgstackIterator iter;
         Node* a = guard(iter.getlast(), &Node::numberp);
         if ( a->fixnump() )
            return Memory::fixnum( a->getfixnum() + 1 );
         else
            return Memory::flonum( a->getflonum() + 1.0 );
      }

      Node* dec()
      {
         ArgstackIterator iter;
         Node* a = guard(iter.getlast(), &Node::numberp);
         if ( a->fixnump() )
            return Memory::fixnum( a->getfixnum() - 1 );
         else
            return Memory::flonum( a->getflonum() - 1.0 );
      }

      Node* abs()
      {
         ArgstackIterator iter;
         Node* a = guard(iter.getlast(), &Node::numberp);
         if ( a->fixnump() )
            return Memory::fixnum( std::abs(a->getfixnum()) );
         else
            return Memory::flonum( fabs(a->getflonum()) );
      }

      Node* gcd()
      {
         ArgstackIterator iter;

         if ( !iter.more() )
            return Memory::fixnum(0);

         FIXNUM n = std::abs( guard(iter.getarg(), &Node::fixnump)->getfixnum() );

         while ( iter.more() ) 
         {
            FIXNUM m = std::abs( guard(iter.getarg(), &Node::fixnump)->getfixnum() );

            while ( true ) 
            {                      /* euclid's algorithm */
               FIXNUM r = m % n;
               if (r == 0)
                  break;
               m = n;
               n = r;
            }
         }
         return Memory::fixnum(n);
      }

      // seed with current time of system
      static FIXNUM rseed = time(NULL);

      static FIXNUM rand( FIXNUM n )
      {
         // make sure pseudo sequence does not get stuck at zero
         if ( rseed == 0L ) 
            rseed = 1L;
 
         // algorithm taken from Dr. Dobbs Journal, November 1985, page 91
         const FIXNUM k1 = rseed / 127773L;
         if ( (rseed = 16807L * (rseed - k1 * 127773L) - k1 * 2836L) < 0L )
            rseed += 2147483647L;
 
         // return a random number between 0 and n-1
         return static_cast<FIXNUM>(rseed % n);
      }

      Node* random()
      {
         ArgstackIterator iter;
         return Memory::fixnum(rand(guard(iter.getlast(), &Node::fixnump)->getfixnum()));
      }

      Node* lognot()
      {
         ArgstackIterator iter;
         const FIXNUM a = guard(iter.getlast(), &Node::fixnump)->getfixnum();
         return Memory::fixnum(static_cast<FIXNUM>(~static_cast<UFIXNUM>(a)));
      }

      Node* rsh()
      {
         ArgstackIterator iter;
         const FIXNUM a = guard(iter.getarg(), &Node::fixnump)->getfixnum();
         const FIXNUM b = guard(iter.getlast(), &Node::fixnump)->getfixnum();
         return Memory::fixnum(static_cast<FIXNUM>(static_cast<UFIXNUM>(a) >> b));
      }

      Node* lsh()
      {
         ArgstackIterator iter;
         const FIXNUM a = guard(iter.getarg(), &Node::fixnump)->getfixnum();
         const FIXNUM b = guard(iter.getlast(), &Node::fixnump)->getfixnum();
         return Memory::fixnum(static_cast<FIXNUM>(static_cast<UFIXNUM>(a) << b));
      }

      Node* ars()
      {
         ArgstackIterator iter;
         const FIXNUM a = guard(iter.getarg(), &Node::fixnump)->getfixnum();
         const FIXNUM b = guard(iter.getlast(), &Node::fixnump)->getfixnum();
         return Memory::fixnum(a >> b);
      }
   }
}
