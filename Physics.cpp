

#include "Physics.h"


Physics::Physics(void): m_car(NULL), m_checkpointOffset(0)
{
    Initialize();
}

Physics::~Physics(void)
{
    Deinitialize();
}

void Physics::Initialize()
{
    ///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
    m_collisionConfiguration = new btDefaultCollisionConfiguration();
    
    ///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
    m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
    
    ///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
    m_overlappingPairCache = new btAxisSweep3(btVector3(-1000,-1000,-1000), btVector3(1000,1000,1000));//new btDbvtBroadphase();
    
    ///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
    m_constraintSolver = new btSequentialImpulseConstraintSolver;
    
    m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_overlappingPairCache, m_constraintSolver, m_collisionConfiguration);
    m_ghostPairCallback = new btGhostPairCallback();
    m_dynamicsWorld->getPairCache()->setInternalGhostPairCallback(m_ghostPairCallback);
   // m_dynamicsWorld->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(m_ghostPairCallback); 
    m_dynamicsWorld->setGravity(btVector3(0, -9.81f, 0));

    m_debugDraw = new PhysicsDebugDraw;    
    m_dynamicsWorld->setDebugDrawer(m_debugDraw);
    
    // checkpoint test
    btCollisionShape* checkpointShape = new btBoxShape(btVector3(2.f, 2.f, 0.1f));
    this->AddCollisionShape(checkpointShape);
    
    m_ghostObject = new btGhostObject();
    m_ghostObject->setCollisionShape(checkpointShape);
    m_ghostObject->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);
    m_dynamicsWorld->addCollisionObject(m_ghostObject);//, btBroadphaseProxy::SensorTrigger, btBroadphaseProxy::AllFilter & ~btBroadphaseProxy::SensorTrigger);
    m_ghostObject->setWorldTransform(PhysicsUtils::btTransFrom(btVector3(0, -3, -5)));
}

void Physics::Deinitialize()  //cleanup in the reverse order of creation/initialization
{
    //remove the rigidbodies from the dynamics world and delete them
    for (int i = m_dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
    {
        btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[i];
        btRigidBody* body = btRigidBody::upcast(obj);
        if (body && body->getMotionState())
            delete body->getMotionState();
        m_dynamicsWorld->removeCollisionObject( obj );
        delete obj;
    }

    //delete collision shapes
    for (int j = 0; j < m_collisionShapes.size(); j++)
        delete m_collisionShapes[j];
    
    //delete m_indexVertexArrays;
    //delete m_vertices;

    //delete dynamics world
    delete m_dynamicsWorld;
            
    if (m_car)
        delete m_car;
    //delete m_vehicleRayCaster;
    //delete m_vehicle;
    //delete m_wheelShape;

    delete m_constraintSolver;
    delete m_overlappingPairCache;
    delete m_dispatcher;
    delete m_collisionConfiguration;
}

void Physics::StepSimulation(btScalar timeStep)
{
    if (m_car)
        m_car->Update(timeStep);
    
    m_dynamicsWorld->stepSimulation(timeStep, 10);

    
    if (m_car)
    for(int i = 0; i < m_ghostObject->getNumOverlappingObjects(); i++)
    {
        btRigidBody *pRigidBody = static_cast<btRigidBody *>(m_ghostObject->getOverlappingObject(i));
        if (pRigidBody == m_car->GetVehicle()->getRigidBody())
        {
            btTransform transform;
            transform.setIdentity();
            m_car->SetLastCheckpointPos(btVector3(0.f, -3.f, -5.f - m_checkpointOffset));
            m_checkpointOffset += 10;
            transform.setOrigin(btVector3(0.f, -3.f, -5.f - m_checkpointOffset));  //-10
            m_ghostObject->setWorldTransform(transform);            
            
            std::cout << "collision";// << std::endl;

        }
        // do whatever you want to do with these pairs of colliding objects
    }
}

btRigidBody * Physics::AddRigidBody(float mass, const btTransform & startTransform, btCollisionShape * shape)
{
    btRigidBody* body = PhysicsUtils::CreateRigidBody(mass, startTransform, shape);
    
    m_dynamicsWorld->addRigidBody(body);

    return body;
}

std::vector<btCollisionShape *> Physics::CreateStaticCollisionShapes(BaseModel * model, float scale)
{
    std::vector<btCollisionShape *> ret;
    
    for (unsigned int i = 0; i < model->getMeshes().size(); i++)
    {
        Mesh *m = model->getMeshes()[i];

        int numTriangles = m->getFaces().size();
        int numVertices = m->getVertices().size();

        if (numTriangles == 0 || numVertices == 0)
            continue;

        btTriangleIndexVertexArray* tiva = new btTriangleIndexVertexArray(numTriangles, (int*)(&m->getFaces()[0]), sizeof(glm::ivec3),
                                                                          numVertices, (btScalar*)(&m->getVertices()[0]), sizeof(glm::vec3));
        
        btBvhTriangleMeshShape *shape = new btBvhTriangleMeshShape(tiva, true);        
        shape->setLocalScaling(btVector3(scale, scale, scale));

        ret.push_back(shape);
    }

    return ret;
}

void Physics::AddStaticModel(std::vector<btCollisionShape *> & collisionShapes, const btTransform & trans, bool debugDraw)
{
    for (unsigned int i = 0; i < collisionShapes.size(); i++)
    {
        btRigidBody *body = AddRigidBody(0, trans, collisionShapes[i]); //->getCollisionFlags;
        if (!debugDraw)
            body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT);
    }
}
