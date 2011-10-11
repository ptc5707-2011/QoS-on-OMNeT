//
// Generated file, do not edit! Created by opp_msgc 4.1 from QueueControllerMessage.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "QueueControllerMessage_m.h"

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




Register_Class(QueueControllerMessage);

QueueControllerMessage::QueueControllerMessage(const char *name, int kind) : cMessage(name,kind)
{
    this->load_var = 0;
    this->windowTime_var = 0;
}

QueueControllerMessage::QueueControllerMessage(const QueueControllerMessage& other) : cMessage()
{
    setName(other.getName());
    operator=(other);
}

QueueControllerMessage::~QueueControllerMessage()
{
}

QueueControllerMessage& QueueControllerMessage::operator=(const QueueControllerMessage& other)
{
    if (this==&other) return *this;
    cMessage::operator=(other);
    this->load_var = other.load_var;
    this->windowTime_var = other.windowTime_var;
    return *this;
}

void QueueControllerMessage::parsimPack(cCommBuffer *b)
{
    cMessage::parsimPack(b);
    doPacking(b,this->load_var);
    doPacking(b,this->windowTime_var);
}

void QueueControllerMessage::parsimUnpack(cCommBuffer *b)
{
    cMessage::parsimUnpack(b);
    doUnpacking(b,this->load_var);
    doUnpacking(b,this->windowTime_var);
}

double QueueControllerMessage::getLoad() const
{
    return load_var;
}

void QueueControllerMessage::setLoad(double load_var)
{
    this->load_var = load_var;
}

double QueueControllerMessage::getWindowTime() const
{
    return windowTime_var;
}

void QueueControllerMessage::setWindowTime(double windowTime_var)
{
    this->windowTime_var = windowTime_var;
}

class QueueControllerMessageDescriptor : public cClassDescriptor
{
  public:
    QueueControllerMessageDescriptor();
    virtual ~QueueControllerMessageDescriptor();

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

Register_ClassDescriptor(QueueControllerMessageDescriptor);

QueueControllerMessageDescriptor::QueueControllerMessageDescriptor() : cClassDescriptor("QueueControllerMessage", "cMessage")
{
}

QueueControllerMessageDescriptor::~QueueControllerMessageDescriptor()
{
}

bool QueueControllerMessageDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<QueueControllerMessage *>(obj)!=NULL;
}

const char *QueueControllerMessageDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int QueueControllerMessageDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 2+basedesc->getFieldCount(object) : 2;
}

unsigned int QueueControllerMessageDescriptor::getFieldTypeFlags(void *object, int field) const
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
    };
    return (field>=0 && field<2) ? fieldTypeFlags[field] : 0;
}

const char *QueueControllerMessageDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "load",
        "windowTime",
    };
    return (field>=0 && field<2) ? fieldNames[field] : NULL;
}

int QueueControllerMessageDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='l' && strcmp(fieldName, "load")==0) return base+0;
    if (fieldName[0]=='w' && strcmp(fieldName, "windowTime")==0) return base+1;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *QueueControllerMessageDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "double",
        "double",
    };
    return (field>=0 && field<2) ? fieldTypeStrings[field] : NULL;
}

const char *QueueControllerMessageDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int QueueControllerMessageDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    QueueControllerMessage *pp = (QueueControllerMessage *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string QueueControllerMessageDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    QueueControllerMessage *pp = (QueueControllerMessage *)object; (void)pp;
    switch (field) {
        case 0: return double2string(pp->getLoad());
        case 1: return double2string(pp->getWindowTime());
        default: return "";
    }
}

bool QueueControllerMessageDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    QueueControllerMessage *pp = (QueueControllerMessage *)object; (void)pp;
    switch (field) {
        case 0: pp->setLoad(string2double(value)); return true;
        case 1: pp->setWindowTime(string2double(value)); return true;
        default: return false;
    }
}

const char *QueueControllerMessageDescriptor::getFieldStructName(void *object, int field) const
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
    };
    return (field>=0 && field<2) ? fieldStructNames[field] : NULL;
}

void *QueueControllerMessageDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    QueueControllerMessage *pp = (QueueControllerMessage *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


