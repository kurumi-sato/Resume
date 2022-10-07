open Funs

(********************************)
(* Part 1: High Order Functions *)
(********************************)

let contains_elem lst e = fold (fun acc x -> if x = e then true else acc) false lst
;;

let is_present lst x = fold (fun acc y -> if x = y then acc@[1] else acc@[0]) [] lst
;;

let count_occ lst target = fold (fun acc x -> if x = target then acc + 1 else acc) 0 lst
;;

let uniq lst = fold (fun acc x -> if (contains_elem acc x = false) then acc@[x] else acc) [] lst
;;

let assoc_list lst = let uniquelist = uniq lst in
  fold (fun acc x -> acc@[x, (count_occ lst x)]) [] (uniquelist)
;;

let ap fns args = fold (fun acc x -> acc@(map x args)) [] fns
;;
