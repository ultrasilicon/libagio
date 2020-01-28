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
  switch (states_.id) {
    case ParsingStates::Start:
      {
        if(buffers_.size() == 0)
          return;
        if(buffers_.front()->length() < sizeof(MsgSizeT)) // header incomplete
          return;

        states_.msg_len = scopeLen<MsgSizeT>(buffers_.front()->data()); // read header
        states_.wbuf = new Buffer(states_.msg_len); // allocate sufficient buffer
        states_.wptr = &(*states_.wbuf)[0];
        Buffer* rbuf = buffers_.front();
        if(rbuf->length() < states_.msg_len + sizeof(MsgSizeT))
          {
            //! read the whole first buffer and destroy
            MsgSizeT readLen = rbuf->length() - sizeof(MsgSizeT);
            memcpy(states_.wptr,
                   scopeBegin<MsgSizeT>(rbuf->data()),
                   readLen);
            buffers_.pop();
            delete rbuf;

            states_.read_len = readLen;
            states_.rptr = buffers_.front()->data();
            states_.id = ParsingStates::ReadingHeader;
          }
        else
          {
            //! read part of the buffer
            memcpy(states_.wptr,
                   scopeBegin<MsgSizeT>(rbuf->data()),
                   states_.msg_len);

            states_.read_len = states_.msg_len;
            states_.rptr = scopeEnd<MsgSizeT>(rbuf->data());
            states_.id = ParsingStates::ReadingBody;
          }

        decode(ev);
        return;
      }
    case ParsingStates::ReadingHeader:
      {

        return;
      }
    case ParsingStates::ReadingBody:
      {
        if(buffers_.size() == 0 || !states_.rptr || !states_.wptr)
          return;
        while (states_.read_len < states_.msg_len && buffers_.size() != 0) {
            Buffer* rbuf = buffers_.front();
            MsgSizeT distance = rbuf->back() - states_.rptr;
            if(states_.read_len + distance < states_.msg_len) // Should switch clauses for better performance
              {
                //! The rest of this buf is a part of the message, read the whole buffer
                MsgSizeT readLen = rbuf->length() - sizeof(MsgSizeT);
                memcpy(states_.wptr,
                       scopeBegin<MsgSizeT>(rbuf->data()),
                       readLen);
                buffers_.pop();
                delete rbuf;

                states_.read_len = readLen;
                states_.rptr = buffers_.front()->data();
                states_.id = ParsingStates::ReadingHeader;
              }
            else
              {
                //! Part of this buf is a part of the message, read part of the buffer
                //! TODO
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

