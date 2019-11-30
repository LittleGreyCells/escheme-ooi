
(define prom1 nil)

(define (test-promises)

  (let ((value 1000))
    (set! prom1 (delay value))
    (eqv? value (force prom1))
    )
)

