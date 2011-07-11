//
// Generated file, do not edit! Created by opp_msgc 4.1 from QoSMessage.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "QoSMessage_m.h"

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// Another default rule (prevents compiler from choosing base class' doPacking())
template<typename T>
void doPacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doPacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}

template<typename T>
void doUnpacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doUnpacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}




Register_Class(QoSMessage);

QoSMessage::QoSMessage(const char *name, int kind) : cPacket(name,kind)
{
    this->from_var = 0;
    this->to_var = 0;
    this->seqCount_var = 0;
}

QoSMessage::QoSMessage(const QoSMessage& other) : cPacket()
{
    setName(other.getName());
    operator=(other);
}

QoSMessage::~QoSMessage()
{
}

QoSMessage& QoSMessage::operator=(const QoSMessage& other)
{
    if (this==&other) return *this;
    cPacket::operator=(other);
    this->from_var = other.from_var;
    this->to_var = other.to_var;
    this->seqCount_var = other.seqCount_var;
    return *this;
}

void QoSMessage::parsimPack(cCommBuffer *b)
{
    cPacket::parsimPack(b);
    doPacking(b,this->from_var);
    doPacking(b,this->to_var);
    doPacking(b,this->seqCount_var);
}

void QoSMessage::parsimUnpack(cCommBuffer *b)
{
    cPacket::parsimUnpack(b);
    doUnpacking(b,this->from_var);
    doUnpacking(b,this->to_var);
    doUnpacking(b,this->seqCount_var);
}

const char * QoSMessage::getFrom() const
{
    return from_var.c_str();
}

void QoSMessage::setFrom(const char * from_var)
{
    this->from_var = from_var;
}

const char * QoSMessage::getTo() const
{
    return to_var.c_str();
}

void QoSMessage::setTo(const char * to_var)
{
    this->to_var = to_var;
}

unsigned long QoSMessage::getSeqCount() const
{
    return seqCount_var;
}

void QoSMessage::setSeqCount(unsigned long seqCount_var)
{
    this->seqCount_var = seqCount_var;
}

class QoSMessageDescriptor : public cClassDescriptor
{
  public:
    QoSMessageDescriptor();
    virtual ~QoSMessageDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(QoSMessageDescriptor);

QoSMessageDescriptor::QoSMessageDescriptor() : cClassDescriptor("QoSMessage", "cPacket")
{
}

QoSMessageDescriptor::~QoSMessageDescriptor()
{
}

bool QoSMessageDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<QoSMessage *>(obj)!=NULL;
}

const char *QoSMessageDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int QoSMessageDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 3+basedesc->getFieldCount(object) : 3;
}

unsigned int QoSMessageDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<3) ? fieldTypeFlags[field] : 0;
}

const char *QoSMessageDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "from",
        "to",
        "seqCount",
    };
    return (field>=0 && field<3) ? fieldNames[field] : NULL;
}

int QoSMessageDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='f' && strcmp(fieldName, "from")==0) return base+0;
    if (fieldName[0]=='t' && strcmp(fieldName, "to")==0) return base+1;
    if (fieldName[0]=='s' && strcmp(fieldName, "seqCount")==0) return base+2;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *QoSMessageDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "string",
        "string",
        "unsigned long",
    };
    return (field>=0 && field<3) ? fieldTypeStrings[field] : NULL;
}

const char *QoSMessageDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int QoSMessageDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    QoSMessage *pp = (QoSMessage *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string QoSMessageDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    QoSMessage *pp = (QoSMessage *)object; (void)pp;
    switch (field) {
        case 0: return oppstring2string(pp->getFrom());
        case 1: return oppstring2string(pp->getTo());
        case 2: return ulong2string(pp->getSeqCount());
        default: return "";
    }
}

bool QoSMessageDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    QoSMessage *pp = (QoSMessage *)object; (void)pp;
    switch (field) {
        case 0: pp->setFrom((value)); return true;
        case 1: pp->setTo((value)); return true;
        case 2: pp->setSeqCount(string2ulong(value)); return true;
        default: return false;
    }
}

const char *QoSMessageDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldStructNames[] = {
        NULL,
        NULL,
        NULL,
    };
    return (field>=0 && field<3) ? fieldStructNames[field] : NULL;
}

void *QoSMessageDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    QoSMessage *pp = (QoSMessage *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


