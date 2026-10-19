theorem six (p q r : Prop)(h:p∧(q∨r)): (p∧q)∨(p∧r) := by
 have hp : p := h.left
 have hqr: q∨r := h.right
 cases hqr with
|  inl hq=>
     have hpq: p∧q := And.intro hp hq
     exact Or.inl hpq
|  inr hr=>
     have hpr: p∧r := And.intro hp hr
     exact Or.inr hpr

theorem five(p q r : Prop)(h : (p ∨q)∨r): p∨(q ∨r):=by
 cases h with
 |inl hpq =>
    cases hpq with
    |inl hp=>
       exact Or.inl hp
    |inr hq=>
       exact Or.inr (Or.inl hq)
 |inr hr =>
     exact Or.inr (Or.inr hr)

theorem four( p q r :Prop) (h: q→r) : p∨q → p∨r :=by
 intro hpq
 cases hpq with
 | inl hp =>
     exact Or.inl hp
 | inr hq =>
     exact Or.inr (h hq)

theorem three(p q r :Prop) (h : p→q) : p∧r → q∧r :=by
 intro hpr
 apply And.intro
 . apply h
   exact hpr.left
 . exact hpr.right

theorem two(p q r : Prop) (h:p→q→r) : (p∧q)→r :=by
 intro hpq
 apply h
 .exact hpq.left
 .exact hpq.right

theorem one ( p q r : Prop) (h : (p∧q)→r) : p→q→r := by -- i have declared
-- a theorem named one , the first: seperates out what we have assumed
-- declared and what we have to proof
-- := by means that we will provide the proof of the theorem using the
-- following tactics(commands) the follwoing are all commands that constitute the proof
 intro hp
 intro hq
 apply h
 constructor
 . exact hp
 . exact hq
