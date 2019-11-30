
(define (test-conversions)

  (assert (equal? (integer->string 10) "10"))
  (assert (equal? (string->integer "-10") -10))
  (assert (equal? (string->integer (integer->string 10)) 10))
)



