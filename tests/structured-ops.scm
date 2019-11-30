(define (test-structured-ops)  
  
  (assert (equal? (cons 1 2) '(1 . 2)))
  (assert (equal? (car (cons 1 2)) 1))
  (assert (equal? (cdr (cons 1 2)) 2))
  
  (assert (equal? (list) '() ))
  (assert (equal? (list 1) '(1) ))
  (assert (equal? (list 1 2) '(1 2) ))
  (assert (equal? (list*) '() ))
  (assert (equal? (list* 1) 1 ))
  (assert (equal? (list* 1 2) '(1 . 2) ))
  (assert (equal? (list* 1 2 3) '(1 2 . 3) ))
  
  (assert (equal? (length (list)) 0))
  (assert (equal? (length (list 1)) 1))
  
  (assert (equal? (set-car! '(1 . 2) 10) '(10 . 2)))
  (assert (equal? (set-cdr! '(1 . 2) 10) '(1 . 10)))
  
  (assert (equal? (vector) #()))
  (assert (equal? (vector 1) #(1)))
  (assert (equal? (vector 1 2) #(1 2)))
  (assert (equal? (make-vector 0) #()))
  (assert (equal? (make-vector 1) #(()) ))
  (assert (equal? (make-vector 2) #(() ()) ))
  
  (set! v (vector 1 2))
  (assert (equal? (vector-ref v 0) 1))
  (assert (equal? (vector-ref v 1) 2))

  (vector-set! v 0 10)
  (assert (equal? (vector-ref v 0) 10))
  (assert (equal? v #(10 2)) )

  (vector-set! v 1 20)
  (assert (equal? v #(10 20)) )
  (assert (equal? (vector-length #()) 0))
  (assert (equal? (vector-length #(1)) 1))
  
  (assert (equal? (vector->list (list->vector '())) '()))
  (assert (equal? (vector->list (list->vector '(1))) '(1)))
  (assert (equal? (vector->list (list->vector '(1 2))) '(1 2)))
  
  (assert (equal? (string->list (list->string '())) '()))
  (assert (equal? (string->list (list->string '(#\a))) '(#\a)))
  (assert (equal? (string->list (list->string '(#\a #\b))) '(#\a #\b)))
)
