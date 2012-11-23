#include <Python.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <fcntl.h>

#include "packet.h"
#include "conn.h"

/*
#include "lz77.h"

static PyObject *lz77(PyObject *self, PyObject *args) {
	const char *payload;
	int len;

	if (!PyArg_ParseTuple(args, "s#", &payload, &len)) 
		return NULL;

	lz_ctxt *c = lz_ctxt_new_full((byte *)payload, len);
	lz77_compress(c);
	int r = c->compressed_data_size;
	lz_ctxt_delete(c);

	return Py_BuildValue("i", r);
}

static PyObject *sumup(PyObject *self, PyObject *args) {
	struct classify_t h[CLASSIFY_HASH_SIZE];
	int i, c;
	packet_sumup(h, &c);
	PyObject *tuple = PyDict_New();
	for (i = 0; i < c; i++) {
		PyDict_SetItem(tuple, 
				Py_BuildValue("zk", h[i].str, h[i].flag), 
				PyLong_FromLong(h[i].cnt)
			);
	}
	return tuple;
}

static PyObject *classify(PyObject *self, PyObject *args) {
	const char *payload;
	int len;
	if (!PyArg_ParseTuple(args, "s#", &payload, &len)) 
		return NULL;
	packet_classify((uint8_t *)payload, len);
	return Py_BuildValue("");
}

*/

static PyObject *_conn_track(PyObject *self, PyObject *args) {
	const char *payload;
	int len;
	if (!PyArg_ParseTuple(args, "s#", &payload, &len)) 
		return NULL;
	packet_t pkt;
	packet_parse(&pkt, (uint8_t *)payload, len);
	conn_track(&pkt);
	return Py_BuildValue("");
}

static PyMethodDef methods[] = {
//	{"lz77", lz77, METH_VARARGS, "lz77 simple method"},
//	{"classify", classify, METH_VARARGS, "classify packet"},
//	{"sumup", sumup, METH_VARARGS, "classify packet sumup"},
	{"conn_track", _conn_track, METH_VARARGS, "packet conn track"},
	{NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC initlibcmdusr() 
{
	Py_InitModule("libcmdusr", methods);
}

int main(int argc, char *argv[])
{
	Py_SetProgramName(argv[0]);
	Py_Initialize();
	initlibcmdusr();
}


