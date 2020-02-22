#include "proto_engine.h"

using namespace Agio;

ProtoEngine::ProtoEngine(TcpSocket* socket, Loop* l)
  : try_parse_(new AsyncEvent(l))
  , socket_(socket)
{
  on(&try_parse_->onCalled, this, &ProtoEngine::decode);
}

ProtoEngine::~ProtoEngine()
{

}

StreamProtoEngine::~StreamProtoEngine()
{

}

void StreamProtoEngine::message(Packet* packet)
{
  write(packet);
}

void StreamProtoEngine::decode(AsyncEvent* ev)
{
  switch (state_) {
    case ParsingState::Start:
      {
        if(buffers_.size() == 0)
          return;
        if(buffers_.front()->length() < sizeof(MsgSizeT)) // header incomplete
          return;

        initStates(scopeLen<MsgSizeT>(buffers_.front()->data()));
        Buffer* rbuf = buffers_.front();
        if(rbuf->length() < totalLength())
          {
            //! read the whole first buffer and destroy
            MsgSizeT readLen = rbuf->length() - sizeof(MsgSizeT);
            readBuf(scopeBegin<MsgSizeT>(rbuf->data()), readLen);
            buffers_.pop();

            rptr_ = buffers_.front()->data();
            state_ = ParsingState::ReadingHeader;
          }
        else
          {
            //! read part of the buffer
            readBuf(scopeBegin<MsgSizeT>(rbuf->data()), msg_len_ - read_len_);

            rptr_ = scopeEnd<MsgSizeT>(rbuf->data());
            state_ = ParsingState::ReadingBody;
          }

        state_ = ParsingState::ReadingBody;


        decode(ev);
        return;
      }
    case ParsingState::ReadingHeader:
      {

        return;
      }
    case ParsingState::ReadingBody:
      {
        if(buffers_.size() == 0 || !rptr_ || !wptr_)
          return;
        while (read_len_ != msg_len_ && buffers_.size() != 0) {
            //! REMOVE: Debug only
            assert(read_len_ <= msg_len_);

            Buffer* rbuf = buffers_.front();
            MsgSizeT distance = rbuf->back() - rptr_;
            if(read_len_ + distance < msg_len_) // Should switch clauses for better performance
              {
                //! The rest of this buf is a part of the message, read the whole buffer
                MsgSizeT readLen = rbuf->length();
                readBuf(scopeBegin<MsgSizeT>(rbuf->data()), readLen);
                buffers_.pop();

                rptr_ = buffers_.front()->data();
                state_ = ParsingState::ReadingHeader;
              }
            else
              {
                //! Part of this buf is a part of the message, read part of the buffer
                readBuf(scopeBegin<MsgSizeT>(rbuf->data()), msg_len_ - read_len_);

                rptr_ = scopeEnd<MsgSizeT>(rbuf->data());
                state_ = ParsingState::ReadingHeader; // ok
              }
          }
        return;
      }

    }
}


void StreamProtoEngine::read(Buffer* stream, TcpSocket*)
{
  buffers_.push(stream);
  try_parse_->send();
}

void StreamProtoEngine::write(Packet* packet)
{
}

void StreamProtoEngine::encode(AsyncEvent* ev)
{
}

