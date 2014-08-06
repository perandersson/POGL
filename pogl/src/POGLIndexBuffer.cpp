#include "MemCheck.h"
#include "POGLIndexBuffer.h"
#include "POGLDeviceContext.h"
#include "POGLSyncObject.h"
#include <atomic>
namespace {
	std::atomic<POGL_UINT32> uid;
	POGL_UINT32 GenIndexBufferUID() {
		return ++uid;
	}
}

POGLIndexBuffer::POGLIndexBuffer(GLuint bufferID, POGL_UINT32 typeSize, POGL_UINT32 numIndices, GLenum type, GLenum bufferUsage, POGLSyncObject* syncObject, IPOGLDevice* device)
: mRefCount(1), mUID(GenIndexBufferUID()), mBufferID(bufferID), mTypeSize(typeSize), mNumIndices(numIndices), mType(type), mBufferUsage(bufferUsage), mSyncObject(syncObject), mDevice(device)
{

}

POGLIndexBuffer::~POGLIndexBuffer()
{
}

void POGLIndexBuffer::AddRef()
{
	mRefCount++;
}

void POGLIndexBuffer::Release()
{
	if (--mRefCount == 0) {
		if (mBufferID != 0) {
			IPOGLDeviceContext* context = mDevice->GetDeviceContext();
			static_cast<POGLDeviceContext*>(context)->DeleteBuffer(mBufferID);
			context->Release();
			mBufferID = 0;
		}
		if (mSyncObject != nullptr) {
			delete mSyncObject;
			mSyncObject = nullptr;
		}
		delete this;
	}
}

IPOGLDevice* POGLIndexBuffer::GetDevice()
{
	mDevice->AddRef();
	return mDevice;
}

POGL_HANDLE POGLIndexBuffer::GetHandlePtr()
{
	return this;
}

void POGLIndexBuffer::WaitSyncDriver()
{
	mSyncObject->WaitSyncDriver();
}

void POGLIndexBuffer::WaitSyncClient()
{
	mSyncObject->WaitSyncClient();
}

bool POGLIndexBuffer::WaitSyncClient(POGL_UINT64 timeout)
{
	return mSyncObject->WaitSyncClient(timeout);
}

bool POGLIndexBuffer::WaitSyncClient(POGL_UINT64 timeout, IPOGLWaitSyncJob* job)
{
	return mSyncObject->WaitSyncClient(timeout, job);
}

POGL_UINT32 POGLIndexBuffer::GetNumElements() const
{
	return mNumIndices;
}

POGL_UINT32 POGLIndexBuffer::GetUID() const
{
	return mUID;
}

GLuint POGLIndexBuffer::GetBufferID() const
{
	return mBufferID;
}

GLenum POGLIndexBuffer::GetType() const
{
	return mType;
}
