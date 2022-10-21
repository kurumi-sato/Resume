open Funs

(*************************************)
(* Part 2: Three-Way Search Tree *)
(*************************************)

type int_tree =
  | IntLeaf
  | IntNode of int * int option * int_tree * int_tree * int_tree 

let empty_int_tree = IntLeaf

let rec int_mem x t = match t with
| IntLeaf -> false
| IntNode (a, b, c, d, e) -> if a = x then true else if b = (Some x) then true
  else if x < a then 
    int_mem x c 
  else if (Some x) > b then 
    int_mem x e 
  else 
    int_mem x d
;;

let rec int_insert x t = match t with
| IntLeaf -> IntNode (x, None, empty_int_tree, empty_int_tree, empty_int_tree)
| IntNode (a, b, c, d, e) -> if int_mem x t = true then t else

  (* If there is only one element in the node *)
  if (b = None) then
    if x < a then IntNode (x, Some a, c, d, e) else IntNode (a, Some x, c, d, e)
  (* If both a and b are filled with integers *)
  else
    if (x < a) then
      IntNode (a, b, int_insert x c, d, e)
    else if (Some x > b) then 
      IntNode (a, b, c, d, int_insert x e)
    else
      IntNode (a, b, c, int_insert x d, e)
;;

let rec int_size t = match t with
| IntLeaf -> 0
| IntNode (a, b, c, d, e) -> if b = None then 1 else 2 + int_size c + int_size d + int_size e
;;

let rec aux_int_max max t = match t with
| IntLeaf -> max
| IntNode (a, b, c, d, e) -> match b with
  | None -> if a < max then aux_int_max max c else aux_int_max a c
  | Some x -> if x > max then aux_int_max x e else aux_int_max max d
;;

let int_max t = match t with
| IntLeaf -> raise (Invalid_argument("int_max"))
| IntNode (a, b, c, d, e) -> aux_int_max 0 t
;;

(*******************************)
(* Part 3: Three-Way Search Tree-Based Map *)
(*******************************)

type 'a tree_map =
  | MapLeaf
  | MapNode of (int * 'a) * (int * 'a) option * 'a tree_map * 'a tree_map * 'a tree_map

let empty_tree_map = MapLeaf

let rec map_contains k t = match t with
  | MapLeaf -> false 
  | MapNode ((a, b), c, d, e, f) -> if a = k then true else match c with 
    | None -> if k < a then map_contains k d else false
    | Some (x, y) -> 

        (* If it goes to the left branch *)
        if (k < a) then
          map_contains k d
          
        (* If it goes to the right branch *)
        else if k > x then
          map_contains k f
        (* If it goes to the middle branch*)    
        else 
          map_contains k e

let rec map_put k v t = match t with
| MapLeaf -> MapNode ((k, v), None, empty_tree_map, empty_tree_map, empty_tree_map)
| MapNode ((a,b), c, d, e, f) -> if map_contains k t then raise (Invalid_argument("map_put")) else 
  match c with
   (* If there is only one element in the node *)
  | None -> if k < a then MapNode ((k, v), Some (a, b), d, e, f) else MapNode ((a, b), Some (k, v), d, e, f)
  (* If both a and b are filled with integers *)
  | Some (x, y) ->
    if (k < a) then
      MapNode ((a,b), Some (x,y), (map_put k v d), e, f)
    else if (k > x) then
      MapNode ((a,b), Some (x,y), d, e, (map_put k v f))
    else
      MapNode ((a,b), Some (x,y), d, (map_put k v e), f)
;;


let rec map_get k t = match t with
| MapLeaf -> raise (Invalid_argument("map_get"))
| MapNode ((a,b), c, d, e, f) -> if a = k then b else match c with
  | None -> if a > k then (map_get k d) else raise (Invalid_argument("map_get"))
  | Some (x, y) -> if x = k then y else if k < x then (map_get k d) else if k > a then (map_get k f) else map_get k e
;;

(***************************)
(* Part 4: Variable Lookup *)
(***************************)

(* Modify the next line to your intended type *)
type lookup_table = (string * int) list list

let empty_table : lookup_table = []

let push_scope (table : lookup_table) : lookup_table = [] :: table
;;

let pop_scope (table : lookup_table) : lookup_table = match table with
| [] -> failwith "No scopes remain!"
| h :: t -> t
;;


let rec duplicate name value lst = match lst with 
  | [] -> true
  | h::t -> match h with
    | (a, b) -> if a = name then false else duplicate name value t
;;

let rec add_var name value (table : lookup_table) : lookup_table = match table with
  | [] -> failwith "There are no scopes to add a variable to!"
  | h::t -> if (duplicate name value h = false) then 
      failwith "Duplicate variable binding in scope!"
    else let append = [(name, value)]@h in 
      [append]@t
;;


let rec lookup_helper name list = match list with
  | [] -> Int.min_int
  | h::t -> if fst h = name then snd h else (lookup_helper name t)
;;

let rec lookup name (table : lookup_table) = match table with
  | [] -> failwith "Variable not found!"
  | h::t -> if lookup_helper name h = min_int then
      lookup name t
    else
      lookup_helper name h
;;
