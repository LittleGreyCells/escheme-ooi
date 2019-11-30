

(define (test-lets)
  (let ((a 1) (b 2))
    (assert (equal? (+ a b) 3))
    )
  (letrec ((a 1) (b a))
    (assert (not (null? b)))
    (if (number? b)
      (assert (equal? (+ a b) 2)))
    )
  (let ((sum 0))
    (let ((n1 10))
      (while (> n1 0)
	(let ((n2 10))
	  (while (> n2 0)
	    (set! sum (+ sum n2))
	    (set! n2 (- n2 1)))
	  (set! n1 (- n1 1)))))
    (assert (equal? sum 550))
    sum)
)

