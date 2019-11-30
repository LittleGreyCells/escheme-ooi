(define (test-property-lists)  
  
  (set-symbol-plist! 'x ())
  (set-symbol-plist! 'foo ())
  (set-symbol-plist! 'bar ())
  
  (assert (begin (set! x 10) (equal? (symbol-value 'x) 10)))
  (assert (begin (set-symbol-value! 'x 20) (equal? (symbol-value 'x) 20)))
  
  (assert (equal? (get 'foo 'x) ()))
  (assert (begin (put 'foo 'x 10) (equal? (get 'foo 'x) 10)))
  (assert (equal? (symbol-plist 'bar) '()) )
  (assert (begin (put 'bar 'x 10) (equal? (symbol-plist 'bar) (list 'x 10))))
  (assert (begin (put 'bar 'y 20) (equal? (symbol-plist 'bar) (list 'y 20 'x 10))))
  )
