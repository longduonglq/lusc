// Project AIDA
// Created by Long Duong on 6/16/22.
// Purpose: 
//

#ifndef LUSCBACKEND_PYEK_H
#define LUSCBACKEND_PYEK_H

#include <pybind11/embed.h>
#include <pybind11/pybind11.h>
#include "wrapper.h"

#ifdef __cplusplus
extern "C" {
#endif

////////// Date //////////

//typedef struct {
//    PyObject_HEAD
//    cx::Date _date;
//} PyDate;
//
//static inline PyDate* PyDate_init_from_time_itv_since(PyTypeObject* cls, PyObject* args, PyObject* kws) {
//    CTimeInterval itv = 0;
//    static const char* kwlist[] = {"since", nullptr};
//    const char* since = nullptr;
//    int ok = PyArg_ParseTupleAndKeywords(args, kws, "d|s", const_cast<char**>(kwlist), &itv, &since);
//    if (!ok) return nullptr;
//    auto* inst = (PyDate*) cls->tp_new(cls, nullptr, nullptr);
//    if (strcmp(since, "now") == 0) inst->_date = cx::Date::init_from_time_interval_since_now(itv);
//    else if (strcmp(since, "reference_date") == 0) inst->_date = cx::Date::init_from_time_interval_since_reference_date(itv);
//    else if (strcmp(since, "1970") == 0) inst->_date = cx::Date::init_from_time_interval_since_1970(itv);
//    else { Py_DECREF(inst); inst = nullptr; }
//    return inst;
//}
//
//static inline PyDate* PyDate_distant_future(PyTypeObject* cls) {
//    auto* inst = (PyDate*) cls->tp_new(cls, nullptr, nullptr);
//    inst->_date = cx::Date::distant_future();
//    return inst;
//}
//
//static inline PyDate* PyDate_distant_past(PyTypeObject* cls) {
//    auto* inst = (PyDate*) cls->tp_new(cls, nullptr, nullptr);
//    inst->_date = cx::Date::distant_past();
//    return inst;
//}
//
//static inline PyObject* PyDate_repr(PyDate* self) {
//    return PyUnicode_FromString(self->_date.formatted().c_str());
//}
//
//static PyMethodDef PyDate_methods[] = {
//        {"init_from_time_itv_since", (PyCFunction)PyDate_init_from_time_itv_since, METH_VARARGS | METH_KEYWORDS | METH_CLASS, "create new Date object from time interval."},
//        {"distant_future", (PyCFunction) PyDate_distant_future, METH_CLASS, "get distant future"},
//        {"distant_past", (PyCFunction) PyDate_distant_past, METH_CLASS, "get distant past."},
//        {nullptr}
//};
//
//static PyTypeObject PyDateType = {
//        PyVarObject_HEAD_INIT(nullptr, 0)
//        .tp_name = "Date",
//        .tp_basicsize = sizeof(PyDate),
//        .tp_itemsize = 0,
//        .tp_new = PyType_GenericNew,
//        .tp_flags = Py_TPFLAGS_DEFAULT,
//        .tp_new = PyType_GenericNew,
//        .tp_methods = PyDate_methods,
//        .tp_repr = (reprfunc)PyDate_repr,
//};
//
//static PyModuleDef lusc_module = {
//        PyModuleDef_HEAD_INIT,
//        .m_name = "ekevent",
//        .m_doc = "Contains EKEvent API",
//
//};

#ifdef __cplusplus
}
#endif

#endif //LUSCBACKEND_PYEK_H
