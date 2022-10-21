# CMSC 420 - Fall 2022 
# HW1 - implementation Ksato1

# Created by Justin Wyss Gallifent and John Kirchenbauer
# based on assignments from previous offerings 
# of CMSC 420 taught by David Mount

# Any extra imports you add may or may not be available 
# in the autograder environment
from typing import List, Tuple
import json
from json import JSONDecodeError

# Do not modify
class Node():
    def  __init__(self, 
                  key        = None, 
                  leftchild  = None,
                  rightchild = None):
        self.key        = key
        self.leftchild  = leftchild
        self.rightchild = rightchild

# Do not modify
def load_tree(json_str: str) -> Node:
    """Takes in a string representation of a tree and 
       constructs a collection of Nodes based on it and 
       returns a reference to the root Node."""
    
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
    
    return _from_dict(dict_repr)


# Do not modify
def dump_tree(root: Node) -> str:
    """Converts a tree represented as a collection of Nodes
        to a string representation and returns the string."""

    def _to_dict(node) -> dict:
        
        return {
            "k": node.key,
            "l": (_to_dict(node.leftchild) if node.leftchild is not None else None),
            "r": (_to_dict(node.rightchild) if node.rightchild is not None else None)
        }
    
    if root == None:
        dict_repr = {}
    else:
        dict_repr = _to_dict(root)

    return json.dumps(dict_repr)


def preorder(root: Node) -> str:

    """ Performs the preorder traversal of tree rooted at 'root'
        and returns a json represntation of the list of keys
        curr, rec left, rec right

        Example return line: return json.dumps(key_list) 
        ... '[250, 351, 420]' """

    def _preorder_list(currlist, node) -> list: 
        if node is None:
            return currlist
        currlist.append(node.key)
        if node.leftchild is None:
            if node.rightchild is None:
                return currlist
            else:
                _preorder_list(currlist, node.rightchild)
                return currlist
        else:
            _preorder_list(currlist, node.leftchild)
            _preorder_list(currlist, node.rightchild)
            return currlist

    lists = []
    if root is None:
        return json.dumps(lists)
    else:
        lists = _preorder_list(lists, root)
    return json.dumps(lists)

 
def inorder(root: Node) -> str:
    """ Performs the inorder traversal of tree rooted at 'root'
        and returns a json represntation of the list of keys
        left node right

        Example return line: return json.dumps(key_list) 
        ... '[250, 351, 420]' """

    def _inorder_list(key_list, node) -> list: 
        if node is None:
            return key_list
        if node.leftchild is None:
            if node.rightchild is None:
                key_list.append(node.key)
                return key_list
            else:
                key_list.append(node.key)
                _inorder_list(key_list, node.rightchild)
                return key_list
        else:
            _inorder_list(key_list, node.leftchild)
            key_list.append(node.key)
            _inorder_list(key_list, node.rightchild)
            return key_list

    lists = []
    if root == None:
        return json.dumps(lists)
    else:
        lists = _inorder_list(lists, root)
    
    return json.dumps(lists)


def postorder(root: Node) -> str:
    """ Performs the postorder traversal of tree rooted at 'root'
        and returns a json represntation of the list of keys
        left right node

        Example return line: return json.dumps(key_list) 
        ... '[420, 351, 250]' """


    def _postorder_list(key_list, node) -> list: 
        if node is None:
            return key_list
        if node.leftchild is None:
            if node.rightchild is None:
                key_list.append(node.key)
                return key_list
            else:
                _postorder_list(key_list, node.rightchild)
                key_list.append(node.key)
                return key_list
        else:
            _postorder_list(key_list, node.leftchild)
            _postorder_list(key_list, node.rightchild)
            key_list.append(node.key)
            return key_list

    lists = []
    if root is None:
        return json.dumps(lists)
    else:
        lists = _postorder_list(lists, root)
    
    return json.dumps(lists)


def insert(root: Node, key: int) -> Node:
    """Returns the BST resulting from inserting the 'key' 
       into the BST rooted at 'root'
       NOTE: if 'key' is already contained in the tree, raise a ValueError"""
    

    if root is None:
        root = Node(key, None, None)
        return root

    elif key == root.key:
        raise ValueError

    elif key < root.key:
            root.leftchild = insert(root.leftchild, key)

    # key > root.key
    else:
        root.rightchild = insert(root.rightchild, key)

    return root


def delete(root: Node, key: int) -> Node:
    """Returns a BST rooted at 'root' with the key 'key' removed.
       NOTE: if the call to delete removes the last key from the tree,
       make sure that the original reference root now refers to None."""
    
    # If not found, raise ValueError

    if root is None:
        return None

    # Please find the node to delete, continue to search
    elif key < root.key:
        root.leftchild = delete(root.leftchild, key)
    elif key > root.key:
        root.rightchild = delete(root.rightchild, key)
    
    # If the key was found
    else:
        # If node that needs to be deleted is a leaf, just cut it off
        if root.leftchild is None and root.rightchild is None:
            return None

        # If it has one one child promote the one child
        elif root.leftchild is None:
            root = root.rightchild
            return root

        elif root.rightchild is None:
            root = root.leftchild
            return root

        # two children
        else:
            smallest = root.rightchild
            while smallest.leftchild != None:
                smallest = smallest.leftchild
            
            root.key = smallest.key

            # now it has been replaced, so lets fix up the tree! The replacement node is now empty with one child

            # this should be the case. so promote the right child
            root.rightchild = delete(root.rightchild, smallest.key)

        return root

    return root