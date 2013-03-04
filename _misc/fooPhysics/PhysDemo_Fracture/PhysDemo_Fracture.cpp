/***************************************************************************************
*  Program Name:    Fracture.cpp                                                       *         
*  Name:            Bryan Sweeney                                                      *
*  Date:            February 14, 2013                                                  *
*  Description:     This file contains the definition file for the default application *
*                   object needed to run PhysDemo_Fracture                             *
*                   - Showcases a different aspect of the Physics Engine: play Demo    *
*                             - Seperate Solution -                                    *
*  Update:           Jen 1/30/2012                                                     *
*                                                                                      *
***************************************************************************************//*
 * The fracture demo.
 */

#include "../PhysDemo_Fracture/gl/glut.h"
#include "../PhysicsEngine/physicsengine/PhysicsEngine.h"
#include "../Common Files/timing.h"
#include "../Common Files/PhysApp.h"
#include "random.h"



#include <stdio.h>

#define MAX_BLOCKS 9

 PhysicsEngine::Random global_random;

class Block : public PhysicsEngine::CollisionBox
{
public:
    bool exists;

    Block()
    :
    exists(false)
    {
        pBody = new PhysicsEngine::PhysBody();
    }

    ~Block()
    {
        delete pBody;
    }

    /** Draws the block. */
    void render()
    {
        // Get the OpenGL transformation
        GLdouble mat[16];
        pBody->getGLTransform(mat);

        if (pBody->getObjectStatis()) glColor3f(1.0f,0.7f,0.7f);
        else glColor3f(0.7f,0.7f,1.0f);

        glPushMatrix();
        glMultMatrixd(mat);
        glScalef(m_v3HalfSizeBox.x*2, m_v3HalfSizeBox.y*2, m_v3HalfSizeBox.z*2);
        glutSolidCube(1.0f);
        glPopMatrix();
    }

    /** Sets the block to a specific location. */
    void setState(const PhysicsEngine::Vector3D &position,
                  const  PhysicsEngine::Quaternion &orientation,
                  const  PhysicsEngine::Vector3D &extents,
                  const  PhysicsEngine::Vector3D &velocity)
    {
        pBody->setBodyPosition(position);
        pBody->setBodyOrientationQ(orientation);
        pBody->setObjectVelocity(velocity);
        pBody->setObjectRotation( PhysicsEngine::Vector3D(0,0,0));
        m_v3HalfSizeBox = extents;

        double mass = m_v3HalfSizeBox.x * m_v3HalfSizeBox.y * m_v3HalfSizeBox.z * 8.0;
        pBody->setBodyMass(mass);

         PhysicsEngine::Matrix3 tensor;
        tensor.SetBlockInertiaTensor(m_v3HalfSizeBox, mass);
        pBody->setInertiaTensor(tensor);

        pBody->setObjectLinearDamping(0.95);
        pBody->setObjectAngularDamping(0.8);
        pBody->emptyAccumulators();
        pBody->setBodyAcceleration(0,-10.0,0);

        //pBody->setCanGoIntoStatis(false);
        pBody->setObjectStatis();

        pBody->calculateObjectData();
    }

    /**
     * Calculates and sets the mass and inertia tensor of this block,
     * assuming it has the given constant density.
     */
    void calculateMassProperties(double invDensity)
    {
        // Check for infinite mass
        if (invDensity <= 0)
        {
            // Just set zeros for both mass and inertia tensor
            pBody->setInverseBodyMass(0);
            pBody->setInverseInertiaTensor( PhysicsEngine::Matrix3());
        }
        else
        {
            // Otherwise we need to calculate the mass
            double volume = m_v3HalfSizeBox.magnitude() * 2.0;
            double mass = volume / invDensity;

            pBody->setBodyMass(mass);

            // And calculate the inertia tensor from the mass and size
            mass *= 0.333;
             PhysicsEngine::Matrix3 tensor;
            tensor.SetInertiaTensorCoeffs(
                mass * m_v3HalfSizeBox.y*m_v3HalfSizeBox.y + m_v3HalfSizeBox.z*m_v3HalfSizeBox.z,
                mass * m_v3HalfSizeBox.y*m_v3HalfSizeBox.x + m_v3HalfSizeBox.z*m_v3HalfSizeBox.z,
                mass * m_v3HalfSizeBox.y*m_v3HalfSizeBox.x + m_v3HalfSizeBox.z*m_v3HalfSizeBox.y
                );
            pBody->setInertiaTensor(tensor);
        }

    }

    /**
     * Performs the division of the given block into four, writing the
     * eight new blocks into the given blocks array. The blocks array can be
     * a pointer to the same location as the target pointer: since the
     * original block is always deleted, this effectively reuses its storage.
     * The algorithm is structured to allow this reuse.
     */
    void divideBlock(const  PhysicsEngine::Contact& contact,
        Block* target, Block* blocks)
    {
        // Find out if we're block one or two in the contact structure, and
        // therefore what the contact normal is.
         PhysicsEngine::Vector3D normal = contact.m_v3NormalofContactWorld;
         PhysicsEngine::PhysBody *pBody = contact.body[0];
        if (pBody != target->pBody)
        {
            normal.invert();
            pBody = contact.body[1];
        }

        // Work out where on the body (in body coordinates) the contact is
        // and its direction.
        PhysicsEngine::Vector3D point = pBody->getPointInLocalSpace(contact.m_v3PointOfContactWorld);
        normal = pBody->getDirectionInLocalSpace(normal);

        // Work out the centre of the split: this is the point coordinates
        // for each of the axes perpendicular to the normal, and 0 for the
        // axis along the normal.
        point = point - normal * (point * normal);

        // Take a copy of the half size, so we can create the new blocks.
        PhysicsEngine::Vector3D size = target->m_v3HalfSizeBox;

        // Take a copy also of the body's other data.
         PhysicsEngine::PhysBody tempBody;
        tempBody.setBodyPosition(pBody->getBodyPosition());
        tempBody.setBodyOrientationQ(pBody->getBodyOrientation());
        tempBody.setObjectVelocity(pBody->getObjectVelocity());
        tempBody.setObjectRotation(pBody->getObjectRotation());
        tempBody.setObjectLinearDamping(pBody->getObjectLinearDamping());
        tempBody.setObjectAngularDamping(pBody->getObjectAngularDamping());
        tempBody.setInverseInertiaTensor(pBody->getInverseInertiaTensor());
        tempBody.calculateObjectData();

        // Remove the old block
        target->exists = false;

        // Work out the inverse density of the old block
        double invDensity =
            m_v3HalfSizeBox.magnitude()*8 * pBody->getInverseBodyMass();

        // Now split the block into eight.
        for (unsigned i = 0; i < 8; i++)
        {
            // Find the minimum and maximum extents of the new block
            // in old-block coordinates
             PhysicsEngine::Vector3D min, max;
            if ((i & 1) == 0) {
                min.x = -size.x;
                max.x = point.x;
            } else {
                min.x = point.x;
                max.x = size.x;
            }
            if ((i & 2) == 0) {
                min.y = -size.y;
                max.y = point.y;
            } else {
                min.y = point.y;
                max.y = size.y;
            }
            if ((i & 4) == 0) {
                min.z = -size.z;
                max.z = point.z;
            } else {
                min.z = point.z;
                max.z = size.z;
            }

            // Get the origin and half size of the block, in old-body
            // local coordinates.
             PhysicsEngine::Vector3D halfSize = (max - min) * 0.5;
             PhysicsEngine::Vector3D newPos = halfSize + min;

            // Convert the origin to world coordinates.
            newPos = tempBody.getPointInWorldSpace(newPos);

            // Work out the direction to the impact.
            PhysicsEngine:: Vector3D direction = newPos - contact.m_v3PointOfContactWorld;
            direction.normalize();

            // Set the body's properties (we assume the block has a body
            // already that we're going to overwrite).
            blocks[i].pBody->setBodyPosition(newPos);
            blocks[i].pBody->setObjectVelocity(tempBody.getObjectVelocity() + direction * 1000.0);
            blocks[i].pBody->setBodyOrientationQ(tempBody.getBodyOrientation());
            blocks[i].pBody->setObjectRotation(tempBody.getObjectRotation());
            blocks[i].pBody->setObjectLinearDamping(tempBody.getObjectLinearDamping());
            blocks[i].pBody->setObjectAngularDamping(tempBody.getObjectAngularDamping());
            blocks[i].pBody->setObjectStatis(true);
            //blocks[i].pBody->setBodyAcceleration( Vector3D::GRAVITY);
            blocks[i].pBody->emptyAccumulators();
            blocks[i].pBody->calculateObjectData();
            blocks[i].m_m4PrimitiveOffset = PhysicsEngine::Matrix4();
            blocks[i].exists = true;
            blocks[i].m_v3HalfSizeBox = halfSize;

            // Finally calculate the mass and inertia tensor of the new block
            blocks[i].calculateMassProperties(invDensity);
        }
    }
};

/**
 * The main demo class definition.
 */
class FractureDemo : public RigidBodyApplication
{
    /** Tracks if a block has been hit. */
    bool hit;
    bool ball_active;
    unsigned fracture_contact;

    /** Handle random numbers. */
     PhysicsEngine::Random random;

    /** Holds the bodies. */
    Block blocks[MAX_BLOCKS];

    /** Holds the projectile. */
    PhysicsEngine::CollisionSphere ball;

    /** Processes the contact generation code. */
    virtual void generateContacts();

    /** Processes the objects in the simulation forward in time. */
    virtual void updateObjects(double dElapsedFrameTime);

    /** Resets the position of all the blocks. */
    virtual void reset();

    /** Processes the physics. */
    virtual void update();

public:
    /** Creates a new demo object. */
    FractureDemo();

    /** Returns the window title for the demo. */
    virtual const char* getTitle();

    /** Display the particle positions. */
    virtual void display();
};

// Method definitions
FractureDemo::FractureDemo()
    :
    RigidBodyApplication()
{
    // Create the ball.
    ball.pBody = new  PhysicsEngine::PhysBody();
    ball.m_dSphereRadius = 0.25;
    ball.pBody->setBodyMass(5.0);
    ball.pBody->setObjectDamping(0.9, 0.9);
     PhysicsEngine::Matrix3 it;
    it.SetDiagonal(5.0, 5.0, 5.0);
    ball.pBody->setInertiaTensor(it);
    //ball.pBody->setBodyAcceleration(Vector3D::GRAVITY);

    ball.pBody->setCanGoIntoStatis(false);
    ball.pBody->setObjectStatis(true);

    // Set up the initial block
    reset();
}

const char* FractureDemo::getTitle()
{
    return "PhysEngine><RABBID>< Fracture Demo";
}

void FractureDemo::generateContacts()
{
    hit = false;

    // Create the ground plane data
    PhysicsEngine:: CollisionPlane plane;
    plane.m_v3direction =  PhysicsEngine::Vector3D(0,1,0);
    plane.m_dPrimitiveOffsetPlane = 0;

    // Set up the collision data structure
    cData.resetContacts(maxContacts);
    cData.m_dFriction = 0.9;
    cData.m_dRestitution = 0.2;
    cData.dCollisionTolerance = 0.1;

    // Perform collision detection
    PhysicsEngine:: Matrix4 transform, otherTransform;
     PhysicsEngine::Vector3D position, otherPosition;
    for (Block *block = blocks; block < blocks+MAX_BLOCKS; block++)
    {
        if (!block->exists) continue;

        // Check for collisions with the ground plane
        if (!cData.hasAnyMoreContacts()) return;
         PhysicsEngine::CollisionDetector::boxAndHalfSpace(*block, plane, &cData);

        if (ball_active)
        {
            // And with the sphere
            if (!cData.hasAnyMoreContacts()) return;
            if ( PhysicsEngine::CollisionDetector::boxAndSphere(*block, ball, &cData))
            {
                hit = true;
                fracture_contact = cData.wNumberOfContacts-1;
            }
        }

        // Check for collisions with each other box
        for (Block *other = block+1; other < blocks+MAX_BLOCKS; other++)
        {
            if (!other->exists) continue;

            if (!cData.hasAnyMoreContacts()) return;
             PhysicsEngine::CollisionDetector::boxAndBox(*block, *other, &cData);
        }
    }

    // Check for sphere ground collisions
    if (ball_active)
    {
        if (!cData.hasAnyMoreContacts()) return;
         PhysicsEngine::CollisionDetector::sphereAndHalfSpace(ball, plane, &cData);
    }
}

void FractureDemo::reset()
{
    // Only the first block exists
    blocks[0].exists = true;
    for (Block *block = blocks+1; block < blocks+MAX_BLOCKS; block++)
    {
        block->exists = false;
    }

    // Set the first block
    blocks[0].m_v3HalfSizeBox = PhysicsEngine:: Vector3D(4,4,4);
    blocks[0].pBody->setBodyPosition(0, 7, 0);
    blocks[0].pBody->setBodyOrientation(1,0,0,0);
    blocks[0].pBody->setObjectVelocity(0,0,0);
    blocks[0].pBody->setObjectRotation(0,0,0);
    blocks[0].pBody->setBodyMass(1.0); //was 100
    PhysicsEngine:: Matrix3 it;
    it.SetBlockInertiaTensor(blocks[0].m_v3HalfSizeBox, 100.0);
    blocks[0].pBody->setInertiaTensor(it);
    blocks[0].pBody->setObjectDamping(0.9, 0.9);
    blocks[0].pBody->calculateObjectData();
    blocks[0].calculatePrimitiveInternals();
	
    //blocks[0].pBody->setBodyAcceleration( Vector3D::GRAVITY);
    blocks[0].pBody->setObjectStatis(true);
    blocks[0].pBody->setCanGoIntoStatis(true);


    ball_active = true;

    // Set up the ball
    ball.pBody->setBodyPosition(0,5.0,20.0);
    ball.pBody->setBodyOrientation(1,0,0,0);
    ball.pBody->setObjectVelocity(
        random.randomBinomial(4.0),
        random.randomDouble(1.0, 6.0),
        -20.0
        );
    ball.pBody->setObjectRotation(0,0,0);
    ball.pBody->calculateObjectData();
    ball.pBody->setObjectStatis(true);
    ball.calculatePrimitiveInternals();

    hit = false;

    // Reset the contacts
    cData.wNumberOfContacts = 0;
}

void FractureDemo::update()
{
    RigidBodyApplication::update();

    // Handle fractures.
    if (hit)
    {
        blocks[0].divideBlock(
            cData.contactArray[fracture_contact],
            blocks,
            blocks+1
            );
        ball_active = true;
    }
}

void FractureDemo::updateObjects(double dElapsedFrameTime)
{
    for (Block *block = blocks; block < blocks+MAX_BLOCKS; block++)
    {
        if (block->exists)
        {
            block->pBody->physicsIntegration(dElapsedFrameTime);
            block->calculatePrimitiveInternals();
        }
    }

    if (ball_active)
    {
        ball.pBody->physicsIntegration(dElapsedFrameTime);
        ball.calculatePrimitiveInternals();
    }
}

void FractureDemo::display()
{
    const static GLfloat lightPosition[] = {0.7f,1,0.4f,0};

    RigidBodyApplication::display();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_NORMALIZE);
    for (Block *block = blocks; block < blocks+MAX_BLOCKS; block++)
    {
        if (block->exists) block->render();
    }
    glDisable(GL_NORMALIZE);

    if (ball_active)
    {
        glColor3f(0.4f, 0.7f, 0.4f);
        glPushMatrix();
         PhysicsEngine::Vector3D pos = ball.pBody->getBodyPosition();
        glTranslatef(pos.x, pos.y, pos.z);
        glutSolidSphere(0.25f, 16, 8);
        glPopMatrix();
    }

    glDisable(GL_LIGHTING);
    glDisable(GL_COLOR_MATERIAL);

    // Draw some scale circles
    glColor3f(0.75, 0.75, 0.75);
    for (unsigned i = 1; i < 20; i++)
    {
        glBegin(GL_LINE_LOOP);
        for (unsigned j = 0; j < 32; j++)
        {
            float theta = 3.1415926f * j / 16.0f;
            glVertex3f(i*cosf(theta),0.0f,i*sinf(theta));
        }
        glEnd();
    }
    glBegin(GL_LINES);
    glVertex3f(-20,0,0);
    glVertex3f(20,0,0);
    glVertex3f(0,0,-20);
    glVertex3f(0,0,20);
    glEnd();

    RigidBodyApplication::drawDebug();
}

/**
 * Called by the common demo framework to create an application
 * object (with new) and return a pointer.
 */
Application* getApplication()
{
    return new FractureDemo();
}