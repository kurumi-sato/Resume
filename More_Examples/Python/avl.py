# CMSC 420 - Fall 2022 
# HW2 - implementation

# Created by Justin Wyss Gallifent and John Kirchenbauer
# based on assignments from previous offerings 
# of CMSC 420 taught by David Mount

# Any extra imports you add may or may not be available 
# in the autograder environment

import json
from typing import List, Text, Union

# the following is available in the autograder env as well
from metaclass_util import LineOfTextMetaclass, NodeMetaclass
# If you don't know what metaclasses are, please ignore it
# as well as the `metaclass=` parameters in the classes definitions below.
# However, if curious, feel free to look inside the file for an explanation.

###############################################################################
# Complete the class below (you don't have to start with this though!) ########
###############################################################################
class LineOfText(metaclass=LineOfTextMetaclass):
    def __init__(self, 
                 line_num : int = None, 
                 text  : str = None):
        self.line_num = int(line_num) # the unique, well ordered, key, cast to int in case is a string
        self.text  = text  # a value field, in our case, a string

    # for printing and dumping
    def __repr__(self):
        return str((self.line_num, f"{self.text[:10]}"))
    def __str__(self):
        return f"Line_{self.line_num}"
    def encode(self):
        return {"line_num":self.line_num, "text": self.text}
    # YOUR CODE HERE
    # Hint: look up "dunder" methods in python
    # __repr__ and __str__ are both "dunder" methods
    # but there is another set of common ones we add 
    # to classes that are used in algorithms
    def __lt__(self, other):
        return self.line_num < other.line_num
    def __gt__(self, other):
        return self.line_num > other.line_num
    def __eq__(self, other):
        return self.line_num == other.line_num


###############################################################################
# Stuff you don't need to modify, but maybe should read #######################
###############################################################################

# Do not modify
class Node(metaclass=NodeMetaclass):
    def  __init__(self, 
                  key        = Union[int, LineOfText],
                  leftchild  = None,
                  rightchild = None,
                  height     = 0):
        self.key        = key
        self.leftchild  = leftchild
        self.rightchild = rightchild
        self.height     = height


# Do not modify
def set_check_heights(root: Node, checking=False) -> None:
    """ Performs the postorder traversal of tree rooted at 'root'
        in order to set / check the heights of nodes.
        hint: does require a properly implemented balance method to operate correctly """

    def _postorder_heights(root: Node):
        if root is None:
            return
        if root.leftchild is not None:
            _postorder_heights(root.leftchild)
        if root.rightchild is not None:
            _postorder_heights(root.rightchild)
        if abs(balance(root)) > 1:
            raise RuntimeError(f"AVL property violated at {root.key} because balance factor is {balance(root)}!")
        
        correct_height = max(height(root.leftchild), height(root.rightchild))+1

        if checking:
            if (root.height != correct_height):
                raise RuntimeError(f"While checking, height of {root.key} was {root.height}, should have been {correct_height}!")
        else:
            root.height = correct_height

        return 
    
    if root is not None:
        _postorder_heights(root)


# Do not modify
# Note, only supports loading trees with integer keys
def load_tree(json_str: str) -> Node:
    
    def _from_dict(dict_repr) -> Node:

        if dict_repr == None or dict_repr == {}:
            return None

        return Node(key=dict_repr["k"],
                    leftchild=_from_dict(dict_repr["l"]),
                    rightchild=_from_dict(dict_repr["r"]))
    
    try: 
        dict_repr = json.loads(json_str)
    except Exception as e:
        print(f"Exception encountered parsing the json string: {json_str}")
        raise e
    
    root = _from_dict(dict_repr)
    set_check_heights(root) # adds correct heights silently but will raise if not balanced
    return root


# Do not modify
class AVLEncoder(json.JSONEncoder):
    def default(self, obj):
        if isinstance(obj, LineOfText): 
        # if hasattr(obj, "text"): 
            return obj.encode()
        return json.JSONEncoder.default(self, obj)


# Do not modify
def dump_tree(root: Node) -> str:

    def _to_dict(node) -> dict:
        
        return {
            "k": node.key,
            "h": node.height,
            "l": (_to_dict(node.leftchild) if node.leftchild is not None else None),
            "r": (_to_dict(node.rightchild) if node.rightchild is not None else None),
        }
    
    if root == None:
        dict_repr = {}
    else:
        dict_repr = _to_dict(root)

    return json.dumps(dict_repr, cls=AVLEncoder)


# Do not modify
def inorder(root: Node, get_text=False) -> str:
    """ Performs the inorder traversal of tree rooted at 'root'
        and returns a json representation of the list of keys. """

    def _inorder(root: Node, key_list: List[int]):
        if root.leftchild is not None:
            _inorder(root.leftchild, key_list)
        key_list.append(root.key)
        if root.rightchild is not None:
            _inorder(root.rightchild, key_list)
    
    key_list = []
    if root is not None:
        _inorder(root, key_list)

    if get_text:
        assert isinstance(root.key, LineOfText)
        # assert hasattr(root.key, "text")
        return [key.text for key in key_list]
    else:
        return json.dumps([str(k) for k in key_list])


# Do not modify, but feel free to use!
def read_text_lines(filepath: str) -> List[LineOfText]:
    lines = []
    with open(filepath, 'r') as fp:
        for line in fp:
            lines.append(line[:-1].split("\t"))
    return lines

###############################################################################
# Your *strongly suggested* methods below #####################################
###############################################################################

def height(root: Node) -> int:
    """return the height of node 'root' which is defined as 
       height of tree rooted at 'root', or longest path from 'root'
       to a leaf. 
       Please follow the convention from Justin's lecture notes.
       Hint: consider the what value to return when root is None."""
    # INSERT CODE
    if root is None:
        return -1
    elif height(root.leftchild) > height(root.rightchild):
        return height(root.leftchild) + 1
    else:
        return height(root.rightchild) + 1

def updateHeight(root: Node):
    root.height = 1 + max(height(root.leftchild), height(root.rightchild))
        

    # SEARCH
def search (root: Node, key) -> int:          
    if root.key is None:
        return None
    elif root.key > key:
        search(root.leftchild, key)
    elif root.key < key:
        search(root.rightchild, key)
    # root.key == key
    else:
        return height

def balance(root: Node) -> str:
    """ return the balance factor at the Node root.
        Please follow the -1,0,1 convention of Justin's lecture notes."""

    # YOUR CODE HERE
    if root is None:
        return 0
 
    else:
        return height(root.rightchild) - height(root.leftchild)


def leftRotate(root) -> Node:
    temp = root.rightchild
    root.rightchild = temp.leftchild
    temp.leftchild = root
    updateHeight(root) # update subtree heights
    updateHeight(temp)
    return temp

def rightRotate(root) -> Node:
    temp = root.leftchild
    root.leftchild = temp.rightchild
    temp.rightchild = root
    updateHeight(root) # update subtree heights
    updateHeight(temp)
    return temp

def rebalance(root) -> Node:
    if root is None:                        # null - nothing to do
        return root
    b = balance(root) 
    if (b < -1): # left heavy?
        if (height(root.leftchild.leftchild) >= height(root.leftchild.rightchild)): # left-left heavy?
            root = rightRotate(root) # fix with single rotation
        else: # left-right heavy?
            root.leftchild = leftRotate(root.leftchild)
            return rightRotate(root)
    elif b > +1:  # right heavy?
        if height(root.rightchild.rightchild) >= height(root.rightchild.leftchild): # right-right heavy
            root = leftRotate(root) # fix with single rotation
        else: # right-left heavy?
            root.rightchild = rightRotate(root.rightchild)
            return leftRotate(root)
    updateHeight(root) # update root height
    return root # return link to updated subtree


###############################################################################
# Your required methods below #################################################
###############################################################################

def insert(root: Node, key: Union[int, LineOfText]) -> Node:
    """Returns the AVL tree resulting from inserting the 'key' 
       into the AVL tree rooted at 'root'
       NOTE: if 'key' is already contained in the tree, raise a ValueError """

    # YOUR CODE HERE

    if root is None:
        return Node(key, None, None, 0)

    elif key == root.key:
        raise ValueError
    elif key > root.key:
        root.rightchild = insert(root.rightchild, key)
    # key < root.key
    else:
        root.leftchild = insert(root.leftchild, key)

    
    return rebalance(root)


    # Recursive function to delete a node with
    # given key from subtree with given root.
    # It returns root of the modified subtree.
def delete(root: Node, key: Union[int, LineOfText]) -> Node: 
    #nothing to delete
    if root is None:
        return root
    elif key < root.key:
        root.leftchild = delete(root.leftchild, key)
    elif key > root.key:
        root.rightchild = delete(root.rightchild, key)
    # if the deleted node is found
    else:
        # case 1: no children
        if root.leftchild is None and root.rightchild is None:
            return None
        # case 2: if only right child
        if root.leftchild is None:
            temp = root.rightchild
            root = None
            return temp
        # case 3: if only left child
        elif root.rightchild is None:
            temp = root.leftchild
            root = None
            return temp
            

        temp = root.rightchild
        while temp.leftchild is not None:
            temp = temp.leftchild
        root.key = temp.key
        root.rightchild = delete(root.rightchild, temp.key)
 
    updateHeight(root)

    return rebalance(root)
 



def text_to_tree(filepath: str) -> Node:
    """Takes a path to a file which contains a collection
       line numbers and strings, creates an AVL tree with the data inserted in the 
       order it appears in the file, and returns the root node.
       Hint: you were given a parsing helper above."""

    # YOUR CODE HERE
    tree = None
    for line in read_text_lines(filepath):
        textline = LineOfText(line_num = line[0], text = line[1])
        tree = insert(tree, textline)

    return tree


def query_text_tree(root: Node, line_num: int) -> str:
    """Search the tree rooted at 'root' for the line number provided 
       returning the text associated with that line_num, or return None if not found.
       The search key, line_num, will always be an integer, 
       but the tree will contain LineOfText objects. """
    
    # YOUR CODE HERE
    if root is None:
        return None
    elif root.key.line_num < line_num:
        return query_text_tree(root.rightchild, line_num)
    elif root.key.line_num > line_num:
        return query_text_tree(root.leftchild, line_num)

    # this one! equals
    else:
        return root.key.text

    return None