#ifndef CCK_GLOBE_H
#define CCK_GLOBE_H

#include <memory>
#include <vector>
#include <queue>

#include "cckBoundBox.h"
#include "cckError.h"
#include "cckGeoCoord.h"
#include "cckSimplexNoise.h"
#include "cckVec3.h"

using std::shared_ptr;
using std::unique_ptr;
using std::vector;

namespace cck
{
    class Globe
    {
    private:

        class Node;
        class Side;
        class Edge;
        class Segment;




        class BspTree
        {
        private:

            class BspNode
            {
            private:
                shared_ptr<Edge>    edge;
                shared_ptr<Node>    node;
                unique_ptr<BspNode> posChild;
                unique_ptr<BspNode> negChild;

            public:
                bool                AddChildren( std::queue<bool>& coord, const shared_ptr<Edge>& newEdge );
                bool                AddNode( std::queue<bool>& coord, const shared_ptr<Node>& newNode );
                bool                IsComplete() const;
                void                SampleData( const cck::GeoCoord& sampleCoord, const cck::Vec3& samplePoint, const double globeRadius, const double noiseValue, double& sampleHeight, int& sampleId ) const;

            public:
                BspNode();
            };

        private:
            BspNode                 root;

        public:
            bool                    AddChildren( std::queue<bool>& coord, const shared_ptr<Edge>& newEdge );
            bool                    AddNode( std::queue<bool>& coord, const shared_ptr<Node>& newNode );
            bool                    IsComplete() const;
            void                    SampleData( const cck::GeoCoord& sampleCoord, const cck::Vec3& samplePoint, const double globeRadius, const double noiseValue, double& sampleHeight, int& sampleId ) const;

        public:
            BspTree();
        };




        class Edge : public std::enable_shared_from_this<Edge>
        {
        private:
            bool                        active;
            vector<shared_ptr<Side>>    sides;  //this should probably be private

        public:
            shared_ptr<Edge>            positiveMountain;
            shared_ptr<Edge>            negativeMountain;
            const shared_ptr<Node>      nodeA;
            const shared_ptr<Node>      nodeB;
            const double                length;
            const shared_ptr<Node>      centerNode;     //nullptr if a mountain Edge
            const cck::Vec3             normal;
            const BspTree               tree;


        private:
            cck::Vec3                   ClosestPoint( const cck::Vec3& samplePoint ) const;
            BspTree                     ConstructTree( const double mountainMinHeight, const double mountainMaxHeight, const double mountainRadius, const double mountainPlateau, const double globeRadius );
            bool                        Contains( const cck::Vec3& point ) const;
            bool                        PointOnFreeSide( const cck::Vec3& samplePoint ) const;

        public:
            void                        AddSides();
            double                      GetDistance( const cck::GeoCoord& sampleCoord, const cck::Vec3& samplePoint, const double globeRadius ) const;
            double                      GetInfluence( const cck::GeoCoord& sampleCoord, const cck::Vec3& samplePoint, const double globeRadius ) const;
            double                      GetMountainHeight( const cck::GeoCoord& sampleCoord, const cck::Vec3& samplePoint, const double globeRadius, const double noiseValue, const double segmentHeight ) const;
            vector<shared_ptr<Side>>    GetSides() const;
            bool                        IsActive() const;
            void                        SampleData( const cck::GeoCoord& sampleCoord, const cck::Vec3& samplePoint, const double globeRadius, const double noiseValue, double& sampleHeight, int& sampleId ) const;
            void                        SetInactive();


        public:
            Edge( const shared_ptr<Node>& nodeA, const shared_ptr<Node>& nodeB, const double mountainMinHeight, const double mountainMaxHeight, const double mountainRadius, const double mountainPlateau, const double globeRadius );
            Edge( const shared_ptr<Node>& nodeA, const shared_ptr<Node>& nodeB, const double globeRadius );
        };





        class Side
        {
        private:
            bool                    formsTriangle;

        public:
            const cck::Vec3         normal;
            const shared_ptr<Edge>  edge;

        public:
            bool                    FormsTriangle() const;
            void                    SetFormsTriangle();

        public:
            Side( const shared_ptr<Node>& nodeA, const shared_ptr<Node>& nodeB, const shared_ptr<Edge>& edge );
        };




        class Link
        {
        public:
            const shared_ptr<Node>  target;
            const shared_ptr<Edge>  edge;

        public:
            bool                    LinksTo( const int nodeId ) const;

        public:
            Link( const shared_ptr<Node>& target, const shared_ptr<Edge>& edge );
        };




        class Node : public std::enable_shared_from_this<Node>
        {
        private:
            vector<shared_ptr<Link>>    links;
            shared_ptr<Segment>         segment;

        public:
            const int                   id;
            const cck::GeoCoord         coord;
            const cck::Vec3             position;
            const cck::Vec3             unitVec;
            const double                minHeight;
            const double                maxHeight;
            const double                radius;
            const double                plateau;
            //const cck::BoundBox       bounds;

        public:
            void                        AddLink( const shared_ptr<Link>& newLink );
            void                        AddToSegment( const shared_ptr<Edge>& newEdge );
            void                        AddToSegment( const shared_ptr<Node>& newNode );
            vector<shared_ptr<Node>>    FindCommonNeighbors( const shared_ptr<Node>& refNode );
            double                      GetDistance( const cck::GeoCoord& sampleCoord, const double globeRadius ) const;
            double                      GetInfluence( const cck::GeoCoord& sampleCoord, const double globeRadius ) const;
            shared_ptr<Link>            GetLinkTo( const int targetId ) const;
            double                      GetMountainHeight( const cck::GeoCoord& sampleCoord, const double globeRadius, const double noiseValue, const double segmentHeight ) const;
            shared_ptr<Segment>         GetSegment() const;
            bool                        LinkedTo( const int nodeId ) const;
            void                        SampleData( const double noiseValue, double& sampleHeight, int& sampleId ) const;

        public:
            Node( const int id, const cck::GeoCoord& coord, const double minHeight, const double maxHeight, const double radius, const double globeRadius );    //Node for continent definitions
            Node( const cck::Vec3& position, const double minHeight, const double maxHeight, const double radius, const double plateau );                       //Node for mountain definitions
        };





        class Triangle
        {
        private:
            const vector<shared_ptr<Node>>  nodes;
            const vector<shared_ptr<Side>>  sides;
            const shared_ptr<Node>          centerNode;
            const BspTree                   tree;
            //const cck::BoundBox           bounds;

        private:
            BspTree                         ConstructTree( const double globeRadius ) const;
            bool                            Contains( const cck::Vec3& point ) const;
            shared_ptr<Node>                CreateCenterNode() const;
            vector<shared_ptr<Node>>        CreateNodeVector( const shared_ptr<Node>& nodeA, const shared_ptr<Node>& nodeB, const shared_ptr<Node>& nodeC ) const;

        public:
            double                          GetInfluence( const cck::Vec3& samplePoint ) const;
            bool                            SampleData( const cck::GeoCoord& sampleCoord, const cck::Vec3& samplePoint, const double globeRadius, const double noiseValue, double& sampleHeight, int& sampleId ) const;


        public:
            Triangle( const shared_ptr<Node>& nodeA, const shared_ptr<Node>& nodeB, const shared_ptr<Node>& nodeC, const vector<shared_ptr<Side>>& sides, const double globeRadius );
        };




        class Segment
        {
        private:
            vector<shared_ptr<Node>>    mountainNodes;
            vector<shared_ptr<Edge>>    mountainEdges;

        public:
            const shared_ptr<Node>      baseNode;

        public:
            void                        AddEdge( const shared_ptr<Edge>& newEdge );
            void                        AddNode( const shared_ptr<Node>& newNode );
            void                        SampleData( const cck::GeoCoord& sampleCoord, const cck::Vec3& samplePoint, const double globeRadius, const double noiseValue, double& sampleHeight, int& sampleId ) const;

        public:
            Segment( const shared_ptr<Node>& baseNode, const vector<shared_ptr<Node>>& mountainNodes, const vector<shared_ptr<Edge>>& mountainEdges );
            Segment( const shared_ptr<Node>& baseNode );
        };




    private:
        double                          globeRadius;
        vector<shared_ptr<Node>>        nodes;
        vector<shared_ptr<Edge>>        edges;
        vector<shared_ptr<Triangle>>    triangles;
        const cck::SimplexNoise         noise;
        int                             noiseOctaves;
        double                          noisePersistance;
        double                          noiseFrequency;

    private:
        static double                   CalculateMountainHeight( const double segmentHeight, const double mountainHeight, const double radius, const double plateau, const double distance );

    public:
        cck::NodeError                  AddNode( const int id, const double latitude, const double longitude, const double minHeight, const double maxHeight, const double nodeRadius );
        cck::NodeError                  AddNode( const int id, const cck::GeoCoord& coord, const double minHeight, const double maxHeight, const double nodeRadius );

        cck::LinkError                  LinkNodes( const int nodeIdA, const int nodeIdB, const double mountainMinHeight, const double mountainMaxHeight, const double mountainRadius, const double mountainPlateau );

        void                            SampleData( const double sampleLatitude, const double sampleLongitude, double& sampleHeight, int& sampleId ) const;
        void                            SampleData( const cck::GeoCoord& sampleCoord, double& sampleHeight, int& sampleId ) const;

        void                            SampleInfluence( const double sampleLatitude, const double sampleLongitude, double& sampleInfluence ) const;
        void                            SampleInfluence( const cck::GeoCoord& sampleCoord, double& sampleInfluence ) const;

        cck::NoiseError                 SetNoiseParameters( const int octaves, const double persistance, const double frequency );

    public:
        Globe( const double radius, const unsigned int seed );
    };
}
#endif // CCK_GLOBE_H
