(define count 0)
(define failures 0)
(define quiet #t)

(define (displayln text)
  (display text)
  (newline))

(define (display-done)
  (newline)
  (newline)
  (displayln "==========================================")
  (displayln "==========================================")
  (displayln "                 DONE!                    ")
  (displayln "==========================================")
  (displayln "==========================================")
  (newline)
  (newline))


(define (assert x)
  (if (not x)
      (begin
       (set! failures (1+ failures))
       )))
