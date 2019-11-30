
(define (test-environment)

  (set! foo (lambda (a b) (the-environment)))
  
  (assert (equal? (environment-bindings (foo 1 2)) '((a . 1) (b . 2))) )
  
  (set! foo (lambda (a b) 
	      (let ((bar (lambda (n)
			   (list a b n))))
		bar)))
  
  (assert (equal? (environment-bindings (procedure-environment (foo 1 2))) 
		   '((a . 1) (b . 2))) )
  
  (set! foo2 (lambda (a b) 
	       (let ((bar (lambda (n)
			    (list a b n))))
		 (the-environment))))
  
  (set! e (foo2 10 20))
  (set! x (environment-bindings e))
  ;;(print x)
  
  (assert (equal? (access a (foo2 1 2)) 1))
  (assert (equal? (access b (foo2 1 2)) 2))
  (assert (equal? ((access bar (foo2 1 2)) 3) '(1 2 3)))
)



