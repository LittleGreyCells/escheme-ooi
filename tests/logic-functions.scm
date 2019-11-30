
(define (test-logical-functions)
  
  (assert (= (logand 1 3) 1))
  (assert (= (logior 1 3) 3))
  (assert (= (logxor 1 3) 2))
  
  ;; 32-bit integers
  ;; ~#xFFFFFFFE --> -2 (or for 64-bit 4294967294)
  
  ;; 64-bit integers
  ;; ~#xFFFFFFFFFFFFFFFE --> -2
  
  (assert (= (lognot 1) -2))
  (assert (= (lognot 1) #xFFFFFFFFFFFFFFFE))
  (assert (= (lognot -2) 1))
  (assert (= (lognot #xFFFFFFFFFFFFFFFE) 1))
  
  (assert (= (shift-right 4 1) 2))
  (assert (= (shift-left 4 1) 8))
  (assert (= (shift-right-arithmetic 2 1) 1))
  (assert (= (shift-right-arithmetic -2 1) -1))
  (assert (= (shift-right-arithmetic #xFFFFFFFFFFFFFFFE 1) #xFFFFFFFFFFFFFFFF))
)


