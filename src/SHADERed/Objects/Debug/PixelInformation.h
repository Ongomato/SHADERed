#pragma once
#include <SHADERed/Engine/Model.h>
#include <SHADERed/Objects/PipelineItem.h>
#include <SHADERed/Objects/ObjectManagerItem.h>

#include <glm/glm.hpp>
#include <unordered_map>

extern "C" {
	#include <spvm/result.h>
}

namespace ed {
	// geometry shader stuff
	enum class GeometryShaderOutput {
		Points,
		LineStrip,
		TriangleStrip
	};
	class GeometryShaderPrimitive {
	public:
		std::vector<glm::vec4> Position;
		std::vector<std::vector<struct spvm_result>> Output;
	};

	class PixelInformation {
	public:
		PixelInformation()
		{
			Color = DebuggerColor = glm::vec4(0.0f);
			Pass = nullptr;
			Object = nullptr;
			Fetched = false;
			Discarded = false;
			VertexCount = 0;
			RenderTexture = nullptr;
			RenderTextureIndex = 0;
			InstanceBuffer = nullptr;
			InstanceID = 0;
			VertexID = 0;
			GeometryOutputType = GeometryShaderOutput::Points;
			GeometryShaderUsed = false;
			GeometrySelectedPrimitive = -1;
			GeometrySelectedVertex = -1;
		}
		glm::vec4 Color;		 // actual pixel color
		glm::vec4 DebuggerColor; // Color generated by the debugger - this way users can see if the SPIRV-VM is executing code correctly...

		PipelineItem* Pass;		   // shader pass responsible for this pixel
		PipelineItem* Object;	   // pipeline item responsible for this pixel


		ObjectManagerItem* RenderTexture; // what render texture
		glm::ivec2 RenderTextureSize; // cache RT size
		int RenderTextureIndex;

		int InTopology, OutTopology; // In == vertex shader input, Out == geometry shader output

		int VertexID;				// gl_VertexID
		int InstanceID;				// gl_InstanceID

		glm::ivec2 Coordinate; // pixel position on the texture
		glm::vec2 RelativeCoordinate;

		bool Fetched;	// Did we fill the DebuggerColor and Vertex[] information for this pixel?
		bool Discarded; // was this pixel discarded?

		int VertexCount;					// 1 for point, 2 for line, 3 for triangle, etc...
		eng::Model::Mesh::Vertex Vertex[3]; // vertices that are responsible for this pixel
		glm::vec4 VertexShaderPosition[3];			// position output from vertex shader
		std::vector<struct spvm_result> VertexShaderOutput[3];

		void* InstanceBuffer;

		// geometry shader stuff
		bool GeometryShaderUsed;
		GeometryShaderOutput GeometryOutputType;
		std::vector<GeometryShaderPrimitive> GeometryOutput;
		int GeometrySelectedPrimitive;
		int GeometrySelectedVertex;

		// final primitive position (equal to VertexShaderPosition if GS is not used, otherwise equal positions from GS's output
		glm::vec4 FinalPosition[3];
	};
}