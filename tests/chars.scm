

(define (test-chars)

  (assert (char-ci=? #\a #\a))
  (assert (not (char-ci=? #\a #\b)))
  
  (assert (char-ci<? #\a #\b))
  (assert (char-ci<=? #\a #\a))
  (assert (char-ci<=? #\a #\b))
  (assert (char-ci>? #\b #\a))
  (assert (char-ci>=? #\b #\b))
  (assert (char-ci>=? #\b #\a))
  
)

