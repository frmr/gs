#ifndef GS_BINARY_SEARCH_TREE
#define GS_BINARY_SEARCH_TREE

#include <memory>

using std::shared_ptr;

namespace gs
{
    template<typename K, typename D>
    class BinarySearchTree
    {
    private:

        class Node
        {
        private:
            K                   key;
            shared_ptr<D>       data;
            shared_ptr<Node>    left;
            shared_ptr<Node>    right;

        public:
            bool Add( const K& newKey, const D& newData )
            {
                if ( newKey < key )
                {
                    if ( left == nullptr )
                    {
                        left = std::make_shared<Node>( newKey, newData );
                        return true;
                    }
                    else
                    {
                        return left->Add( newKey, newData );
                    }
                }
                else if ( newKey > key )
                {
                    if ( right == nullptr )
                    {
                        right = std::make_shared<Node>( newKey, newData );
                        return true;
                    }
                    else
                    {
                        return right->Add( newKey, newData );
                    }
                }
                else
                {
                    return false;
                }
            }

            bool Contains( const K& refKey )
            {
                if ( refKey < key )
                {
                    return ( left == nullptr ) ? false : left->Contains( refKey );
                }
                else if ( refKey > key )
                {
                    return ( right == nullptr ) ? false : right->Contains( refKey );
                }
                else
                {
                    return true;
                }
            }

            shared_ptr<D> GetData( const K& refKey ) const
            {
                if ( refKey < key )
                {
                    return ( left == nullptr ) ? nullptr : left->GetData( refKey );
                }
                else if ( refKey > key )
                {
                    return ( right == nullptr ) ? nullptr : right->GetData( refKey );
                }
                else
                {
                    return data;
                }
            }

        public:
            Node( const K& key, const D& data )
                :   key( key ), data( std::make_shared<D>( data ) ), left( nullptr ), right ( nullptr )
            {
            }
        };

    private:
        shared_ptr<Node>    root;

    public:
        bool Add( const K& newKey, const D& newData )
        {
            if ( root == nullptr )
            {
                root = std::make_shared<Node>( newKey, newData );
                return true;
            }
            else
            {
                return root->Add( newKey, newData );
            }
        }

        bool Contains( const K& refKey ) const
        {
            return ( root == nullptr ) ? false : root->Contains( refKey );
        }

        shared_ptr<D> GetData( const K& refKey ) const
        {
            return ( root == nullptr ) ? nullptr : root->GetData( refKey );
        }

    public:
        BinarySearchTree()
            : root( nullptr )
        {
        }
    };
}

#endif // GS_BINARY_SEARCH_TREE
