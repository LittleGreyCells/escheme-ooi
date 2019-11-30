
(define (test-strings)

  (set! s1 "abc")
  (set! s2 "abc")
  (set! s3 "axy")
  
  (assert (equal? (string-length s1) 3))
  (assert (equal? (string-append s1 s2) "abcabc"))
  (assert (equal? (string-ref s1 0) #\a))
  
  (assert (equal? (substring s1 0 1) "a"))
  (assert (equal? (substring s1 1 0) ""))
  (assert (equal? (substring s1 1 1) ""))
  (assert (equal? (substring s1 1 2) "b"))
  (assert (equal? (substring s1 1 3) "bc"))
  (assert (equal? (substring s1 0 (string-length s1)) s1))
  
  ;;
  ;; string comparison -- case senstive
  ;;
  
  (assert (string=? s1 s1))
  (assert (string=? s1 s2))
  (assert (not (string=? s1 s3)))
  
  (assert (string<? "abc" "abx"))
  (assert (string<? "abc" "ax"))
  (assert (string<? "abc" "x"))
  (assert (string<? "" "a"))
  (assert (not (string<? "a" "a")))
  (assert (not (string<? "a" "")))
  
  (assert (string<=? "abc" "abx"))
  (assert (string<=? "abc" "ax"))
  (assert (string<=? "abc" "x"))
  (assert (string<=? "" "a"))
  (assert (string<=? "a" "a"))
  (assert (not (string<=? "a" "")))
  
  (assert (string>? "abx" "abc"))
  (assert (string>? "ax" "abc"))
  (assert (string>? "x" "abc"))
  (assert (string>? "a" ""))
  (assert (not (string>? "a" "a")))
  (assert (not (string>? "" "a")))
  
  (assert (string>=? "abx" "abc"))
  (assert (string>=? "ax" "abc"))
  (assert (string>=? "x" "abc"))
  (assert (string>=? "a" "a"))
  (assert (not (string>=? "" "a")))
  
  ;;
  ;; string comparison -- case insenstive
  ;;
  
  (assert (string-ci=? "abc" "abc"))
  (assert (string-ci=? "abc" "ABC"))
  (assert (not (string-ci=? "AbC" "AxY")))
  
  (assert (string-ci<? "abc" "ABX"))
  (assert (string-ci<? "abc" "AX"))
  (assert (string-ci<? "abc" "X"))
  (assert (string-ci<? "" "A"))
  (assert (not (string-ci<? "a" "A")))
  (assert (not (string-ci<? "a" "")))
  
  (assert (string-ci<=? "abc" "ABX"))
  (assert (string-ci<=? "abc" "AX"))
  (assert (string-ci<=? "abc" "X"))
  (assert (string-ci<=? "" "A"))
  (assert (string-ci<=? "a" "A"))
  (assert (not (string-ci<=? "a" "")))
  
  (assert (string-ci>? "abx" "ABC"))
  (assert (string-ci>? "ax" "ABC"))
  (assert (string-ci>? "x" "ABC"))
  (assert (string-ci>? "a" ""))
  (assert (not (string-ci>? "a" "A")))
  (assert (not (string-ci>? "" "A")))
  
  (assert (string-ci>=? "abx" "ABC"))
  (assert (string-ci>=? "ax" "ABC"))
  (assert (string-ci>=? "x" "ABC"))
  (assert (string-ci>=? "a" "A"))
  (assert (not (string-ci>=? "" "A")))
  
  ;; swap case of 1st and 2nd arguments
  
  (assert (string-ci=? "ABC" "abc"))
  (assert (not (string-ci=? "ABC" "axy")))
  
  (assert (string-ci<? "ABC" "abx"))
  (assert (string-ci<? "ABC" "ax"))
  (assert (string-ci<? "ABC" "x"))
  (assert (string-ci<? "" "A"))
  (assert (not (string-ci<? "A" "a")))
  (assert (not (string-ci<? "A" "")))
  
  (assert (string-ci<=? "ABC" "abx"))
  (assert (string-ci<=? "ABC" "ax"))
  (assert (string-ci<=? "ABC" "x"))
  (assert (string-ci<=? "" "a"))
  (assert (string-ci<=? "A" "a"))
  (assert (not (string-ci<=? "A" "")))
  
  (assert (string-ci>? "ABX" "abc"))
  (assert (string-ci>? "AX" "abc"))
  (assert (string-ci>? "X" "abc"))
  (assert (string-ci>? "A" ""))
  (assert (not (string-ci>? "A" "a")))
  (assert (not (string-ci>? "" "a")))
  
  (assert (string-ci>=? "ABX" "abc"))
  (assert (string-ci>=? "AX" "abc"))
  (assert (string-ci>=? "X" "abc"))
  (assert (string-ci>=? "A" "a"))
  (assert (not (string-ci>=? "" "a")))
)






