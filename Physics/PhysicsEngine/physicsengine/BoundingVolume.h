/***************************************************************************************
*  Program Name:    BoundingVolume.h                                                   *         
*  Name:            Bryan Sweeney                                                      *
*  Date:            February 14, 2013                                                  *
*  Description:     This file contains the rough first step collision detection system.*
*                   It is used to return pairs of objects that may be in contact, which*
*                   can then be tested using more detailed methods found in            *
*                   CollisionDetect.                                                   *
*  Update:           Jen 1/30/2012                                                     *
*                                                                                      *
***************************************************************************************/
// Interface file for the rough first step collision detection system.

/**
  This file contains the rough first step collision detection system.  It is
 * used to return pairs of objects that may be in contact, which can
 * then be tested using more detailed methods found in CollisionDetect.
 */



#ifndef PHYSICSENGINE_BOUNDINGVOLUME_H_
#define PHYSICSENGINE_BOUNDINGVOLUME_H_

#include <vector>
#include "ContactResolution.h"
#include "physbody.h"

namespace PhysicsEngine 
{

    // Represents a bounding sphere that can be tested for overlap.
     
    struct BoundingSphere
    {
        Vector3D m_v3SphereCenter;
        double m_dSphereRadius;

    public:
        // Creates a new bounding sphere at the given centre and radius.
         
        BoundingSphere(const Vector3D &v3SphereCenter, double dSphereRadius);

        // Creates a bounding sphere to enclose the two given bounding spheres.
        
        BoundingSphere(const BoundingSphere &one, const BoundingSphere &two);

        /**
         * Checks if the bounding sphere overlaps with the other given
         * bounding sphere.
         */
        bool overlaps(const BoundingSphere *other) const;

        /**
         * Reports how much this bounding sphere would have to grow
         * by to incorporate the given bounding sphere. Note that this
         * calculation returns a value not in any particular units (i.e.
         * its not a volume growth). In fact the best implementation
         * takes into account the growth in surface area (after the
         * Goldsmith-Salmon algorithm for tree construction).
         */
        double getGrowth(const BoundingSphere &other) const;

        /**
         * Returns the volume of this bounding volume. This is used
         * to calculate how to recurse into the bounding volume tree.
         * For a bounding sphere it is a simple calculation.
         */
        double getSize() const
        {
            return ((double)1.333333) * PI * m_dSphereRadius * 
				m_dSphereRadius * m_dSphereRadius;
        }
    };

    // Stores a potential contact to check later.
     
    struct PotentialContact
    {
        // Holds the bodies that might be in contact.
         
        PhysBody* pBody[2];
    };

    // A base class for nodes in a bounding volume hierarchy.     
    // This class uses a binary tree to store the bounding volumes.
    
    template<class BoundingVolumeClass>
    class BoundingVolumeNode
    {
    public:
        // Holds the child nodes of this node.
         
        BoundingVolumeNode * children[2];

        /**
         * Holds a single bounding volume encompassing all the
         * descendents of this node.
         */
        BoundingVolumeClass volume;

        /**
         * Holds the Physics body at this node of the hierarchy.
         * Only leaf nodes can have a rigid body defined (see isLeaf).
         * Note that it is possible to rewrite the algorithms in this
         * class to handle objects at all levels of the hierarchy,
         * but the code provided ignores this vector unless firstChild
         * is NULL.
         */
        PhysBody *pBody;

        // Holds the node immediately above us in the tree.
         
        BoundingVolumeNode *pParent;

        // Creates a new node in the hierarchy with the given parameters.
         
        BoundingVolumeNode(BoundingVolumeNode *pParent, const BoundingVolumeClass &volume,
            PhysBody* pBody=NULL)
            : parent(parent), volume(volume), pBody(pBody)
        {
            children[0] = children[1] = NULL;
        }

        // Checks if this node is at the bottom of the hierarchy.
         
        bool isLeaf() const
        {
            return (pBody != NULL);
        }

        /**
         * Checks the potential contacts from this node downwards in
         * the hierarchy, writing them to the given array (up to the
         * given limit). Returns the number of potential contacts it
         * found.
         */
        unsigned getPotentialContacts(PotentialContact* contacts,
                                      unsigned limit) const;

        /**
         * Inserts the given Physics body, with the given bounding volume,
         * into the hierarchy. This may involve the creation of
         * further bounding volume nodes.
         */
        void insert(PhysBody* pBody, const BoundingVolumeClass &volume);

        /**
         * Deletes this node, removing it first from the hierarchy, along
         * with its associated
         * Physics body and child nodes. This method deletes the node
         * and all its children (but obviously not the Physics bodies). This
         * also has the effect of deleting the sibling of this node, and
         * changing the parent node so that it contains the data currently
         * in that sibling. Finally it forces the hierarchy above the
         * current node to reconsider its bounding volume.
         */
        ~BoundingVolumeNode();

    protected:

        /**
         * Checks for overlapping between nodes in the hierarchy. Note
         * that any bounding volume should have an overlaps method implemented
         * that checks for overlapping with another object of its own type.
         */
        bool overlaps(const BoundingVolumeNode<BoundingVolumeClass> *other) const;

        /**
         * Checks the potential contacts between this node and the given
         * other node, writing them to the given array (up to the
         * given limit). Returns the number of potential contacts it
         * found.
         */
        unsigned getPotentialContactsWith(
            const BoundingVolumeNode<BoundingVolumeClass> *other,
            PotentialContact* contacts,
            unsigned limit) const;

        /**
         * For non-leaf nodes, this method recalculates the bounding volume
         * based on the bounding volumes of its children.
         */
        void recalculateBoundingVolume(bool recurse = true);
    };

    // Note that, because we're dealing with a template here, we
    // need to have the implementations accessible to anything that
    // imports this header.
    
    template<class BoundingVolumeClass>
    bool BoundingVolumeNode<BoundingVolumeClass>::overlaps(
        const BoundingVolumeNode<BoundingVolumeClass> * other
        ) const
    {
        return volume->overlaps(other->volume);
    }

    template<class BoundingVolumeClass>
    void BoundingVolumeNode<BoundingVolumeClass>::insert(
        PhysBody* newpBody, const BoundingVolumeClass &newVolume
        )
    {
        // If we are a leaf, then the only option is to spawn two
        // new children and place the new body in one.
        if (isLeaf())
        {
            // Child one is a copy of us.
            children[0] = new BoundingVolumeNode<BoundingVolumeClass>(
                this, volume, pBody
                );

            // Child two holds the new body
            children[1] = new BoundingVolumeNode<BoundingVolumeClass>(
                this, newVolume, newpBody
                );

            // And we now loose the body (we're no longer a leaf)
            this->pBody = NULL;

            // We need to recalculate our bounding volume
            recalculateBoundingVolume();
        }

        // Otherwise we need to work out which child gets to keep
        // the inserted body. We give it to whoever would grow the
        // least to incorporate it.
        else
        {
            if (children[0]->volume.getGrowth(newVolume) <
                children[1]->volume.getGrowth(newVolume))
            {
                children[0]->insert(newBody, newVolume);
            }
            else
            {
                children[1]->insert(newBody, newVolume);
            }
        }
    }

    template<class BoundingVolumeClass>
    BoundingVolumeNode<BoundingVolumeClass>::~BoundingVolumeNode()
    {
        // If we don't have a parent, then we ignore the sibling
        // processing
        if (pParent)
        {
            // Find our sibling
            BoundingVolumeNode<BoundingVolumeClass> *sibling;
            if (parent->children[0] == this) sibling = pParent->children[1];
            else sibling = parent->children[0];

            // Write its data to our parent
            pParent->volume = sibling->volume;
            pParent->pBody = sibling->pBody;
            pParent->children[0] = sibling->children[0];
            pParent->children[1] = sibling->children[1];

            // Delete the sibling (we blank its parent and
            // children to avoid processing/deleting them)
            sibling->pParent = NULL;
            sibling->pBody = NULL;
            sibling->children[0] = NULL;
            sibling->children[1] = NULL;
            delete sibling;

            // Recalculate the parent's bounding volume
            parent->recalculateBoundingVolume();
        }

        // Delete our children (again we remove their
        // parent data so we don't try to process their siblings
        // as they are deleted).
        if (children[0]) {
            children[0]->pParent = NULL;
            delete children[0];
        }
        if (children[1]) {
            children[1]->pParent = NULL;
            delete children[0];
        }
    }

    template<class BoundingVolumeClass>
        void BoundingVolumeNode<BoundingVolumeClass>::recalculateBoundingVolume(
        bool recurse
        )
    {
        if (isLeaf()) return;

        // Use the bounding volume combining constructor.
        volume = BoundingVolumeClass(
            children[0]->volume,
            children[1]->volume
            );

        // Recurse up the tree
        if (pParent) pParent->recalculateBoundingVolume(true);
    }

    template<class BoundingVolumeClass>
    unsigned BoundingVolumeNode<BoundingVolumeClass>::getPotentialContacts(
        PotentialContact* contacts, unsigned limit
        ) const
    {
        // Early out if we don't have the room for contacts, or
        // if we're a leaf node.
        if (isLeaf() || limit == 0) return 0;

        // Get the potential contacts of one of our children with
        // the other
        return children[0]->getPotentialContactsWith(
            children[1], contacts, limit
            );
    }

    template<class BoundingVolumeClass>
    unsigned BoundingVolumeNode<BoundingVolumeClass>::getPotentialContactsWith(
        const BoundingVolumeNode<BoundingVolumeClass> *other,
        PotentialContact* contacts,
        unsigned limit
        ) const
    {
        // Early out if we don't overlap or if we have no room
        // to report contacts
        if (!overlaps(other) || limit == 0) return 0;

        // If we're both at leaf nodes, then we have a potential contact
        if (isLeaf() && other->isLeaf())
        {
            contacts->pBody[0] = pBody;
            contacts->pBody[1] = other->pBody;
            return 1;
        }

        // Determine which node to descend into. If either is
        // a leaf, then we descend the other. If both are branches,
        // then we use the one with the largest size.
        if (other->isLeaf() ||
            (!isLeaf() && volume->getSize() >= other->volume->getSize()))
        {
            // Recurse into ourself
            unsigned count = children[0]->getPotentialContactsWith(
                other, contacts, limit
                );

            // Check we have enough slots to do the other side too
            if (limit > count) {
                return count + children[1]->getPotentialContactsWith(
                    other, contacts+count, limit-count
                    );
            } else {
                return count;
            }
        }
        else
        {
            // Recurse into the other node
            unsigned count = getPotentialContactsWith(
                other->children[0], contacts, limit
                );

            // Check we have enough slots to do the other side too
            if (limit > count) {
                return count + getPotentialContactsWith(
                    other->children[1], contacts+count, limit-count
                    );
            } else {
                return count;
            }
        }
    }

} 

#endif 