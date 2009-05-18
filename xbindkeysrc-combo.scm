;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Start of xbindkeys configuration ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; This configuration is guile based.
;;
;; This configuration allow combo keys.
;; ie Control+z Control+e -> xterm
;;    Control+z z         -> rxvt
;;    Control+z Control+g -> quit second mode
;;
;; It also allow to add or remove a key on the fly!




(define (display-n str)
    "Display a string then newline"
  (display str)
  (newline))


(define (first-binding)
    "First binding"
  (xbindkey '(control shift q) "xterm")
  (xbindkey-function '(control a)
		     (lambda ()
		       (display "Hello from Scheme!")
		       (newline)))
  (xbindkey-function '(shift p)
		     (lambda ()
		       (run-command "xterm")))
  ;; set directly keycode (here shift + m with my keyboard)
  (xbindkey-function '("m:0x1"  "c:47") 
		     (lambda ()
		       (display "------ Adding control k -----")
		       (newline)
		       (xbindkey '(control k) "rxvt")
		       (grab-all-keys)))
  (xbindkey-function '(shift i)
		     (lambda ()
		       (display "Remove control k")
		       (newline)
		       (remove-xbindkey '(control k))
		       (grab-all-keys)))
  (xbindkey-function '(shift o)
		     (lambda ()
		       (display "Remove control a")
		       (newline)
		       (remove-xbindkey '(control a))
		       (grab-all-keys)))
  (xbindkey-function '(control z) second-binding))
		       


(define (reset-first-binding)
    "reset first binding"
  (display-n "reset first binding")
  (ungrab-all-keys)
  (remove-all-keys)
  (first-binding)
  (grab-all-keys))

(define (second-binding)
    "Second binding"
  (display "New binding")
  (ungrab-all-keys)
  (remove-all-keys)
  (xbindkey-function '(control e)
		     (lambda ()
		       (display-n "Control e")
		       (run-command "xterm")
		       (reset-first-binding)))
  (xbindkey-function 'z
		     (lambda ()
		       (display-n "z (second)")
		       (run-command "rxvt")
		       (reset-first-binding)))
  (xbindkey-function '(control g) reset-first-binding)
  (debug)
  (grab-all-keys))
		       



(first-binding)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; End of xbindkeys configuration ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
