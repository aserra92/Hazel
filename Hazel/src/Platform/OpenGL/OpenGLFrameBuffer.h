#pragma once
#include "Hazel/Renderer/FrameBuffer.h"

namespace Hazel {

	class OpenGLFrameBuffer : public FrameBuffer 
	{
	public:
		OpenGLFrameBuffer(const FrameBufferSpecification& spec);
		virtual ~OpenGLFrameBuffer();

		void Invalidate();
		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void Resize(uint32_t width, uint32_t height) override;

		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override { HZ_CORE_ASSERT(index < m_ColorAttachments.size()); return m_ColorAttachments[index]; }

		virtual const FrameBufferSpecification& GetSpecification() const override { return m_Specification;	}
	private:
		uint32_t m_RendererID = 0;
		FrameBufferSpecification m_Specification;

		std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecifications;
		FramebufferTextureSpecification m_DepthAttachmentSpecification;

		std::vector<uint32_t> m_ColorAttachments;
		uint32_t m_DepthAttachment;
	};
}
