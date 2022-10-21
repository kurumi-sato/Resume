open List
open Sets

(*********)
(* Types *)
(*********)

type ('q, 's) transition = 'q * 's option * 'q

type ('q, 's) nfa_t = {
  sigma: 's list;
  qs: 'q list;
  q0: 'q;
  fs: 'q list;
  delta: ('q, 's) transition list;
}

(***********)
(* Utility *)
(***********)

(* explode converts a string to a character list *)
let explode (s: string) : char list =
  let rec exp i l =
    if i < 0 then l else exp (i - 1) (s.[i] :: l)
  in
  exp (String.length s - 1) []

(* Checks through the list if any of them contain the same elements *)
let rec contain lst1 lst2 = match lst1 with
| [] -> false
| h :: t -> if (List.mem h lst2) then true else contain t lst2
;;

(****************)
(* Part 1: NFAs *)
(****************)

(* Check if it matches the transition you are looking for*)
let rec move_helper delta q s = 
  match delta with 
    (* Return a list *)
  | [] -> []
  | h::t -> match h with 
      (* The move matches the state and transition that matches *)
    | (q0,transition,f) -> if (q0 = q && transition = s) then 
      union [f] (move_helper t q s)
    (* Account for epsilon *)
    else if transition = None then
      move_helper t q s
    (* Move through the list *)
    else move_helper t q s
;;

(* Move through every transition in the list *)
let rec move nfa qs s = 
    match qs with 
  | [] -> []
  | h::t -> union (move_helper nfa.delta h s) (move nfa t s)
;;

let rec e_closure_helper nfa qs lst = 
  match qs with
  | [] -> []
  (* If curr element is not in the list then add the eclosure to the list, otherwise dontinue through the list *)
  | h :: t -> if (List.mem h lst) = false then
      (* *)
      h :: (e_closure_helper nfa (append t (move_helper nfa.delta h None)) (append lst [h]))
    else 
      e_closure_helper nfa t lst
  ;;

let e_closure (nfa: ('q,'s) nfa_t) (qs: 'q list) : 'q list =
  e_closure_helper nfa qs []
;;

let rec aux_accept nfa state chars = match chars with
| [] -> let movelst = (e_closure nfa state) in
  if contain nfa.fs movelst then true else false
| h :: t -> aux_accept nfa (move nfa (e_closure nfa state) (Some h)) t
;;

let accept (nfa: ('q,char) nfa_t) (s: string) : bool = 
  aux_accept nfa [nfa.q0] (explode s)
;;

(*******************************)
(* Part 2: Subset Construction *)
(*******************************)

let new_states (nfa: ('q,'s) nfa_t) (qs: 'q list) : 'q list list =
  List.fold_left (fun acc x -> 
    insert (e_closure nfa (move nfa qs (Some x))) acc) [] nfa.sigma
;;

let new_trans (nfa: ('q,'s) nfa_t) (qs: 'q list) : ('q list, 's) transition list =
  List.fold_left (fun acc x -> 
    insert (qs, Some x, e_closure nfa (move nfa qs (Some x))) acc) [] nfa.sigma

let rec new_finals (nfa: ('q,'s) nfa_t) (qs: 'q list) : 'q list list = 
  List.fold_left (fun acc x -> 
    if elem x nfa.fs then [qs] else acc) [] qs
;;

let rec nfa_to_dfa_step (nfa: ('q,'s) nfa_t) (dfa: ('q list, 's) nfa_t)
    (work: 'q list list) : ('q list, 's) nfa_t = match work with
  | [] -> dfa
  | h::t -> let states = new_states nfa h in
      let curr = {
        sigma = dfa.sigma;
        qs = insert_all dfa.qs states ;
        q0 = dfa.q0;
        fs = insert_all dfa.fs (new_finals nfa h);
        delta = insert_all dfa.delta (new_trans nfa h)
      }
      in nfa_to_dfa_step nfa curr (insert_all (minus curr.qs dfa.qs) t)

let nfa_to_dfa (nfa: ('q,'s) nfa_t) : ('q list, 's) nfa_t =
  let dfa = {
    sigma = nfa.sigma;
    qs = [e_closure nfa [nfa.q0]];
    q0 = e_closure nfa [nfa.q0];
    fs = [];
    delta = []
  }
  in nfa_to_dfa_step nfa dfa dfa.qs
