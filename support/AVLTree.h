#pragma once


////	AVL TREE Implementation c++				////
////			using templates					////
////	NickName:	Ice_Cold					////
////	 
////			How-to Use:						////
////	Declare an AVLTree < type >				////
////	where type is ANY builtin or custom		////
////	variable that supports the following	////
////	operators : =, <, >, ==, <<				////
////	and has copy constructor				////


#include <cstdlib>
#include <cstring>
#include <sstream>
#include <iostream>

using namespace std;



template <class T>
class Node
{
public:
    Node<T>():data(),left(),right(),parent(){ }
    Node<T>(T& data_):left(),right(),parent(){
		data = new  T(data_);
	}
	
	T* data;
	Node* left, *right, *parent;
	/// VIRTUAL FUNCTIONS DO NOT WORK ON TEMPLATED CLASSES 
	/// inheritance though works, the bare minimum , that is
	/// so NO inheritance used for the node 
	
};





template <class T>
class AVLTree
{

public:
    //only one extra variable
    Node<T>* root;

    AVLTree():root(){}
	~AVLTree(){}


	int insert(T );
    Node<T>*& search(T );
	int remove(T );
    int exists(T );

	T get_Min(bool to_Be_Removed=false);// will return the leftmost node
	T get_Max(bool to_Be_Removed = false);// will return the rightmost node

	void display();
	std::string toString_Inorder();

private:
	int insert(T& data, Node<T>*&node ,Node<T>*& parent);
    Node<T>*& search(T& data, Node<T>*& node);
	int remove(T& data, Node<T>*& node);
    int exists(T& data, Node<T>*&node,int counter);



	int height(Node<T>*&);
	int balance_factor(Node<T>*& node){ return height(node->left) - height(node->right); }

	void balance_node(Node<T>*& node);
	void balance_node_postOrder(Node<T>*& node)
	{
		if (node == NULL)
			return;		
		else
		{
			balance_node_postOrder(node->left);
			balance_node_postOrder(node->right);
			balance_node(node);
		}
	}
	void balance();
	
	void rot_left(Node<T>*& node);
	void rot_right(Node<T>*& node);
	void rot_rightLeft(Node<T>*& node);
	void rot_leftRight(Node<T>*& node);

	void reparent(Node<T>*& node,Node<T>*& parent);//not used //

	bool isLeftChild(Node<T>*&node){	return (node->parent->left == node);	} //check if they point to the same address in memory
	

	Node<T>*& find_leftMost(Node<T>*&node);//returns the leftmost/minValue Node of  a subTree // SUCCESOR if find_leftMost(tree->right) // if NULL then parent is the successor
	Node<T>*& find_rightMost(Node<T>*&node);//PREDECCESSOR if find_rightMost(tree->left)

	void display(Node<T>* subTree, int depth);

	std::string  toString_Inorder(Node<T>*& node);


};



template <class T>
int AVLTree<T>::exists(T data)
{
    int counter = 0;
    return exists(data, root, counter);
}

template <class T>
int AVLTree<T>::exists(T& data,Node<T>*& node, int counter)
{
    if (node == NULL) return counter;

    if (data == (*node->data))
    {
        /// if there are multiple instances allowed , this if should call search again
        /// on the side the insert would have put it.
        return ++counter;
    }
    if (data < (*node->data))
    {
        return exists(data, node->left,counter);
    }
    else
    {
        return exists(data, node->right,counter);
    }
}

template <class T>
int AVLTree<T>::insert(T  data)
{
	if (root== NULL)
	{
		root = new Node<T>(data);
		//root->parent = NULL;
	}
	else
	{
		insert(data, root,root->parent);//root->parent == NULL
		balance_node(root);
	}

	///balance();

	return 0;
}

template <class T>
int AVLTree<T>::insert(T&  data,Node<T>*& node,Node<T>*& parent)
{
	if (node == NULL)
	{
		//T* data_copy = new T(data);//if i dont do this , it uses THE SAME OBJECT given,
		//						   //for gods sake ,initializer list problem?
		//node = new Node<T>(*data_copy);
	
		node = new Node<T>(data);

		node->parent = parent;
		int breakPoint = 0;

	}
	else if (data < (*node->data))
	{
		insert(data, node->left,node);
		balance_node(node);
	}
	else if (data == (*node->data))
	{
		// in here you can dissallow duplicate data
		//insert left or right or not, whatever
		//i chose Not
	}
	else
	{
		insert(data, node->right,node);
		balance_node(node);
	}

	return 0;
}


template <class T>
Node<T>*& AVLTree<T>::search(T data)
{
    return search(data, root);
}

template <class T>
Node<T>*& AVLTree<T>::search(T& data, Node<T>*& node)
{
    if (node==NULL)
        return node;//its NULL anyway

    else if(data > (*node->data))
        return  search(data,node->right);

    else if(data < (*node->data))
        return  search(data,node->left);
    else if (data == (*node->data))
        return node;
    return node;
}

template <class T>
int AVLTree<T>::remove(T data)
{
if (root == NULL)
		return -1;
	int return_value = remove(data, root);
	//balance();

	return return_value;
}

template <class T>
int AVLTree<T>::remove(T& data, Node<T>*& node)
{
	/// remove function NOT optimal;

	if (node == NULL)
		return -1;//keep looking
	if (data < (*node->data))
	{
		remove(data, node->left);
		balance_node(node);
	}
	else if (data > (*node->data) )
	{
		remove(data, node->right);
		balance_node(node);
	}
	
	/// determine if leaf child
	else if (node->left == NULL && node->right == NULL)
	{
		if (node == root)
		{
			Node<T>* backup_ptr = root;
			root = NULL;
			delete backup_ptr;
			return 0;
		}
		Node<T>* temp = node;

		if (node == root)//if it s root ,delete it
			delete root;
		else if (isLeftChild(node))
			node->parent->left = NULL;
		else
			node->parent->right = NULL;

		delete temp;
	}
	// determine if only right Child exists
	else if (node->left == NULL)
	{
		if (node == root)
		{
			Node<T>* backup_ptr = root;
			root = root->right;
			delete backup_ptr;
			return 0;
		}

		Node<T>* temp = node;

		if (isLeftChild(node))
		{
			node->right->parent = node->parent;
			node->parent->left = node->right;

		}
		else
		{
			node->right->parent = node->parent;
			node->parent->right = node->right;
		}

		delete temp;
	}
	//determine if only left child exists
	else if (node->right == NULL)
	{
		if (node == root)
		{
			Node<T>* backup_ptr = root;
			root = root->left;
			delete backup_ptr;
			return 0;
		}

		Node<T>* temp = node;

		if (isLeftChild(node))
		{
			node->left->parent = node->parent;
			node->parent->left = node->left;
		}
		else
		{
			node->left->parent = node->parent;
			node->parent->right = node->left;
		}

		delete temp;
	}
	//BOTH CHILDREN EXIST
	else
	{
		
		Node<T>* swap_node=find_leftMost(node->right);

		T* data_backup_ptr = node->data;
		node->data = swap_node->data;
		swap_node->data = data_backup_ptr;

		remove(data, node->right);
		balance_node(node);
	}

	return 0;
}


template<class T>
T AVLTree<T>::get_Min(bool toBeRemoved)
{
	if (root == NULL)
		return T();//default constructor, user should take care for empty tree

	T* min_ptr = find_leftMost(root)->data;
	T temp = (* min_ptr); //find leftmost returns pointer. so it needs dereferencig

	if (toBeRemoved)
	{
		remove(temp);//since i know the node to be removed , this should be made optimized deletion
	}
	return temp;
}
template<class T>
T AVLTree<T>::get_Max(bool remove)
{
	if (root == NULL)
		return T();//default constructor, user should take care for empty tree
	
	T* max_ptr = find_rightMost(root)->data;

	T temp = (*max_ptr);

	if (toBeRemoved)
	{
		remove(temp);//since i know the node to be removed , this should be made optimized deletion
	}
	return temp;
}



template <class T>
int AVLTree<T>::height(Node<T>*& node)
{
	if (node != NULL)
	{
		int h_l = height(node->left);
		int h_r = height(node->right);

		return ( (h_l > h_r ? h_l : h_r) + 1);

	}
	else
		return 0;
}

template <class T>
void AVLTree<T>::balance_node(Node<T>*&node)
{
	int factor = balance_factor(node);

	if (factor > 1)//== 2, always, if >1
	{
		if (balance_factor(node->left) > 0)
			rot_left(node);
		else
			rot_leftRight(node);
	}
	else if (factor < -1)//==-2,
	{
		if (balance_factor(node->right) > 0)
			rot_rightLeft(node);
		else
			rot_right(node);
	}
	else; //do nothing
}

template <class T>
void AVLTree<T>::balance()
{
	balance_node_postOrder(root->left);//recursive
	balance_node_postOrder(root->right);//recursive 
	balance_node(root);//non-recursive
}//whole tree with pre-order accessing

template <class T>
void AVLTree<T>::rot_left(Node<T>*& node)
{
	Node<T>* temp = node->left;

	node->left = temp->right;
	temp->right = node;
	node = temp;

	//reparent
	node->parent = node->right->parent;
	node->right->parent = node;
	
	if (node->left->right != NULL)
		node->right->left->parent = node->right;

	//reparent(node, node->parent);

}
template <class T>
void AVLTree<T>::rot_right(Node<T>*& node)
{
	Node<T>* temp = node->right;
	
	node->right = temp->left;
	temp->left = node;
	node = temp;

	//reparent
	node->parent = node->left->parent;//new node->left == previous node
	node->left->parent = node;
	
	if (node->left->right!=NULL)
		node->left->right->parent = node->left;

	//reparent(node, node->parent);

}

template <class T>
void AVLTree<T>::rot_leftRight(Node<T>*& node)
{
	rot_right(node->left);
	rot_left(node);

	reparent(node, node->parent);

}

template <class T>
void AVLTree<T>::rot_rightLeft(Node<T>*& node)
{
	rot_left(node->right);
	rot_right(node);

	reparent(node, node->parent);
}

template <class T>
void AVLTree<T>::reparent(Node<T>*& node, Node<T>*& parent)
{
	if (node == NULL)
		return;
	if (node->left != NULL)
	{
		reparent(node->left, node);
		node->left->parent=node;
	}
	if (node->right != NULL)
	{
		reparent(node->right, node);
		node->right->parent = node;
	}
	
}

template<class T>
Node<T>*& AVLTree<T>::find_leftMost(Node<T>*& node)
{
		if (node->left == NULL)
			return node;
		else
			return find_leftMost(node->left);
}

template<class T>
Node<T>*& AVLTree<T>::find_rightMost(Node<T>*& node)
{
		if (node->right == NULL)
			return node;
		else
			return find_rightMost(node->right);
}



template <class T>
void AVLTree<T>::display()
{
	if (root == NULL)
		return;


	display(root->right, 1);

	std::cout << endl << "ROOT >>\t" << (*root->data)<< endl;
	
	display(root->left, 1);

	std::cout << endl << endl<<endl;


}

template <class T>
void AVLTree<T>::display(Node<T>* subTree, int depth)
{
	if (subTree == NULL)
		return;

	display(subTree->right, depth + 1);

	cout << " " << endl ;
	for (int i = 0; i < depth && subTree != root; i++)
		cout << "\t\t\t" ;
	cout << (*subTree->data) << endl; 

	display(subTree->left, depth + 1);
	
}

template <class T>
std::string AVLTree<T>::toString_Inorder()
{
	return toString_Inorder(root);
}


template <class T>
std::string AVLTree<T>::toString_Inorder(Node<T>*& node)
{
	if (node == NULL)
	{
		return std::string("");
	}
	else
	{
		std::stringstream stream;

		stream << toString_Inorder(node->left);
		stream << "\t" << node->data->toString() << std::endl;
		stream << toString_Inorder(node->right);

		return stream.str();
	}
}
