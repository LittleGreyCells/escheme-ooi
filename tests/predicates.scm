
(define (test-predicates)
  
  (assert (not #f))
  
  (assert (bound? 'foo2))
  (assert (not (bound? 'foo3)))
  
  (assert (null? ()))
  (assert (not (null? 'a)))
  
  (assert (atom? ()))
  (assert (atom? 1))
  (assert (atom? 1.0))
  (assert (atom? "string"))
  (assert (atom? #\a))
  (assert (atom? #()))
  (assert (not (atom? (list 1))))
  
  (assert (list? ()))
  (assert (list? (cons 1 2)))
  (assert (list? (list 1 2 3 4)))
  (assert (not (list? 1)))
  
  (assert (pair? (cons 1 2)))
  (assert (not (pair? '())))
  
  (assert (number? 1))
  (assert (number? 1.0))
  (assert (not (number? "abc")))
  (assert (not (number? #())))
  
  (assert (boolean? #t))
  (assert (boolean? #f))
  (assert (not (boolean? 1)))
  
  (assert (symbol? 'a))
  (assert (not (symbol? 1)))
  
  (assert (integer? 1))
  (assert (not (integer? 1.0)))
  (assert (real? 1.0))
  (assert (not (real? 1)))
  
  (assert (char? #\a))
  (assert (not (char? "a")))
  
  (assert (not (string? #\a)))
  (assert (string? "a"))
  
  (assert (vector? #()))
  (assert (not (vector? '(1 2))))
  
  (assert (byte-vector? (byte-vector 1 2)))
  (assert (not (byte-vector? #())))
  
  (assert (closure? foo2))
  (assert (not (closure? car)))
  
  (assert (procedure? car))
  (assert (procedure? foo2))
  
  (assert (not (environment? '())) )
  (assert (environment? (foo2 1 2)))
  
  (assert (zero? 0))
  (assert (not (zero? 10)))
  (assert (positive? 1))
  (assert (not (positive? 0)))
  (assert (not (positive? -1)))
  (assert (negative? -1))
  (assert (not (negative? 0)))
  (assert (not (negative? 1)))
  
  (assert (odd? 1))
  (assert (odd? 51))
  (assert (odd? -51))
  (assert (not (odd? 0)))
  (assert (even? 0))
  (assert (even? -50))
  (assert (not (even? 1)))
)

