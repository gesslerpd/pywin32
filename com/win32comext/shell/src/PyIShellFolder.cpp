// This file implements the IShellFolder Interface and Gateway for Python.
// Generated by makegw.py

#include "shell_pch.h"
#include "PyIShellFolder.h"

// @doc - This file contains autoduck documentation
// ---------------------------------------------------
//
// Interface Implementation

PyIShellFolder::PyIShellFolder(IUnknown *pdisp) : PyIUnknown(pdisp) { ob_type = &type; }

PyIShellFolder::~PyIShellFolder() {}

/* static */ IShellFolder *PyIShellFolder::GetI(PyObject *self) { return (IShellFolder *)PyIUnknown::GetI(self); }

// @pymethod tuple|PyIShellFolder|ParseDisplayName|Returns the PIDL of an item in a shell folder
// @rdesc The result is a tuple of cchEaten, pidl, attr
// @tupleitem 0|int|cchEaten|the number of characters of the input name that were parsed
// @tupleitem 1|<o PyIDL>|pidl|specifies the relative path from the parsing folder to the object
// @tupleitem 2|int|Attributes|returns any requested attributes
PyObject *PyIShellFolder::ParseDisplayName(PyObject *self, PyObject *args)
{
    IShellFolder *pISF = GetI(self);
    if (pISF == NULL)
        return NULL;
    // @pyparm <o PyHANDLE>|hwndOwner||Window in which to display any dialogs or message boxes, can be 0
    // @pyparm <o PyIBindCtx>|pbc||Bind context that affects how parsing is performed, can be None
    // @pyparm str|DisplayName||Display name to parse, format is dependent on the shell folder.
    // Desktop folder will accept a file path, as well as guids of the form ::{guid}
    // Example: '::%s\\::%s' %(shell.CLSID_MyComputer,shell.CLSID_ControlPanel)
    // @pyparm int|Attributes|0|Combination of shellcon.SFGAO_* constants specifying which attributes should be returned
    PyObject *obpbc;
    PyObject *obDisplayName;
    HWND hwndOwner;
    PyObject *obhwndOwner;
    IBindCtx *pbc;
    TmpWCHAR DisplayName;
    ULONG pchEaten = (ULONG)-1;
    ITEMIDLIST *ppidl;
    ULONG dwAttributes = 0;
    if (!PyArg_ParseTuple(args, "OOO|k:ParseDisplayName", &obhwndOwner, &obpbc, &obDisplayName, &dwAttributes))
        return NULL;
    if (!PyWinObject_AsHANDLE(obhwndOwner, (HANDLE *)&hwndOwner))
        return NULL;
    if (!PyWinObject_AsWCHAR(obDisplayName, &DisplayName))
        return NULL;
    if (!PyCom_InterfaceFromPyInstanceOrObject(obpbc, IID_IBindCtx, (void **)&pbc, TRUE /* bNoneOK */))
        return NULL;

    HRESULT hr;
    PY_INTERFACE_PRECALL;
    hr = pISF->ParseDisplayName(hwndOwner, pbc, DisplayName, &pchEaten, &ppidl, &dwAttributes);
    if (pbc)
        pbc->Release();
    PY_INTERFACE_POSTCALL;

    if (FAILED(hr))
        return PyCom_BuildPyException(hr, pISF, IID_IShellFolder);
    PyObject *obppidl;

    obppidl = PyObject_FromPIDL(ppidl, TRUE);
    PyObject *pyretval = Py_BuildValue("lOl", pchEaten, obppidl, dwAttributes);
    Py_XDECREF(obppidl);
    return pyretval;
}

// @pymethod <o PyIEnumIDList>|PyIShellFolder|EnumObjects|Creates an enumerator to list the contents of the shell folder
PyObject *PyIShellFolder::EnumObjects(PyObject *self, PyObject *args)
{
    IShellFolder *pISF = GetI(self);
    if (pISF == NULL)
        return NULL;
    // @pyparm <o PyHANDLE>|hwndOwner|None|Window to use if any user interaction is required
    // @pyparm int|grfFlags|SHCONTF_FOLDERS\|SHCONTF_NONFOLDERS\|SHCONTF_INCLUDEHIDDEN|Combination of shellcon.SHCONTF_*
    // constants
    HWND hwndOwner = 0;
    PyObject *obhwndOwner = Py_None;
    DWORD grfFlags = SHCONTF_FOLDERS | SHCONTF_NONFOLDERS | SHCONTF_INCLUDEHIDDEN;
    IEnumIDList *ppeidl;
    if (!PyArg_ParseTuple(args, "|Ol:EnumObjects", &obhwndOwner, &grfFlags))
        return NULL;
    if (!PyWinObject_AsHANDLE(obhwndOwner, (HANDLE *)&hwndOwner))
        return NULL;

    HRESULT hr;
    PY_INTERFACE_PRECALL;
    hr = pISF->EnumObjects(hwndOwner, grfFlags, &ppeidl);

    PY_INTERFACE_POSTCALL;

    if (FAILED(hr))
        return PyCom_BuildPyException(hr, pISF, IID_IShellFolder);

    return PyCom_PyObjectFromIUnknown(ppeidl, IID_IEnumIDList, FALSE);
}

// @pymethod <o PyIShellFolder>|PyIShellFolder|BindToObject|Returns an IShellFolder interface for a subfolder
PyObject *PyIShellFolder::BindToObject(PyObject *self, PyObject *args)
{
    IShellFolder *pISF = GetI(self);
    if (pISF == NULL)
        return NULL;
    // @pyparm <o PyIDL>|pidl||Relative item id list that identifies the subfolder, can be multi-level
    // @pyparm <o PyIBindCtx>|pbc||Bind context to be used, can be None
    // @pyparm <o PyIID>|riid||IID of the desired interface, usually IID_IShellFolder
    PyObject *obpidl;
    PyObject *obpbcReserved;
    PyObject *obriid;
    ITEMIDLIST *pidl;
    IBindCtx *pbcReserved;
    IID riid;
    void *out;
    if (!PyArg_ParseTuple(args, "OOO:BindToObject", &obpidl, &obpbcReserved, &obriid))
        return NULL;
    if (!PyWinObject_AsIID(obriid, &riid))
        return NULL;
    if (!PyObject_AsPIDL(obpidl, &pidl))
        return NULL;
    if (!PyCom_InterfaceFromPyInstanceOrObject(obpbcReserved, IID_IBindCtx, (void **)&pbcReserved,
                                               TRUE /* bNoneOK */)) {
        PyObject_FreePIDL(pidl);
        return NULL;
    }

    HRESULT hr;
    PY_INTERFACE_PRECALL;
    hr = pISF->BindToObject(pidl, pbcReserved, riid, &out);
    PyObject_FreePIDL(pidl);
    if (pbcReserved)
        pbcReserved->Release();

    PY_INTERFACE_POSTCALL;

    if (FAILED(hr))
        return PyCom_BuildPyException(hr, pISF, IID_IShellFolder);

    return PyCom_PyObjectFromIUnknown((IUnknown *)out, riid, FALSE);
}

// @pymethod interface|PyIShellFolder|BindToStorage|Returns an interface to a storage object in a shell folder
// @rdesc Returns <o PyIStream>, <o PyIStorage> or <o PyIPropertySetStorage> depending on the riid passed in
PyObject *PyIShellFolder::BindToStorage(PyObject *self, PyObject *args)
{
    IShellFolder *pISF = GetI(self);
    if (pISF == NULL)
        return NULL;
    // @pyparm <o PyIDL>|pidl||Relative pidl for the folder item, must be a single item id
    // @pyparm <o PyIBindCtx>|pbc||Bind context that affects how binding is performed, can be None
    // @pyparm <o PyIID>|riid||IID of the desired interface, one of IID_IStream, IID_IStorage, IID_IPropertySetStorage
    PyObject *obpidl;
    PyObject *obpbcReserved;
    PyObject *obriid;
    ITEMIDLIST *pidl;
    IBindCtx *pbcReserved;
    IID riid;
    void *out;
    if (!PyArg_ParseTuple(args, "OOO:BindToStorage", &obpidl, &obpbcReserved, &obriid))
        return NULL;
    BOOL bPythonIsHappy = TRUE;
    if (bPythonIsHappy && !PyObject_AsPIDL(obpidl, &pidl))
        bPythonIsHappy = FALSE;
    if (bPythonIsHappy &&
        !PyCom_InterfaceFromPyInstanceOrObject(obpbcReserved, IID_IBindCtx, (void **)&pbcReserved, TRUE /* bNoneOK */))
        bPythonIsHappy = FALSE;
    if (!PyWinObject_AsIID(obriid, &riid))
        bPythonIsHappy = FALSE;
    if (!bPythonIsHappy)
        return NULL;
    HRESULT hr;
    PY_INTERFACE_PRECALL;
    hr = pISF->BindToStorage(pidl, pbcReserved, riid, &out);
    PyObject_FreePIDL(pidl);
    if (pbcReserved)
        pbcReserved->Release();

    PY_INTERFACE_POSTCALL;

    if (FAILED(hr))
        return PyCom_BuildPyException(hr, pISF, IID_IShellFolder);

    return PyCom_PyObjectFromIUnknown((IUnknown *)out, riid, FALSE);
}

// @pymethod int|PyIShellFolder|CompareIDs|Determines the sorting order of 2 items in shell folder
// @rdesc Returns 0 if items compare equal, -1 if the pidl1 comes first, or 1 if pidl2 comes first
PyObject *PyIShellFolder::CompareIDs(PyObject *self, PyObject *args)
{
    IShellFolder *pISF = GetI(self);
    if (pISF == NULL)
        return NULL;
    // @pyparm int|lparam||Lower 16 bits specify folder-dependent sorting rules, 0 means to sort by display name.
    // System folder view uses these as a column number.<nl>
    // Upper sixteen bits is used for flags SHCIDS_ALLFIELDS or SHCIDS_CANONICALONLY
    // @pyparm <o PyIDL>|pidl1||Item id list that idenfies an object relative to the folder
    // @pyparm <o PyIDL>|pidl2||Item id list that idenfies an object relative to the folder
    PyObject *obpidl1;
    PyObject *obpidl2;
    PyObject *ret = NULL;
    PyObject *oblparam;
    PyWin_PARAMHolder lparam;
    ITEMIDLIST *pidl1 = NULL;
    ITEMIDLIST *pidl2 = NULL;
    if (!PyArg_ParseTuple(args, "OOO:CompareIDs", &oblparam, &obpidl1, &obpidl2))
        return NULL;

    if (!PyWinObject_AsPARAM(oblparam, &lparam))
        return NULL;

    if (PyObject_AsPIDL(obpidl1, &pidl1) && PyObject_AsPIDL(obpidl2, &pidl2)) {
        HRESULT hr;
        PY_INTERFACE_PRECALL;
        hr = pISF->CompareIDs(lparam, pidl1, pidl2);
        PY_INTERFACE_POSTCALL;
        if (FAILED(hr))
            PyCom_BuildPyException(hr, pISF, IID_IShellFolder);
        else  // special handling of hresult
            if ((short)HRESULT_CODE(hr) < 0)
            /* pidl1 comes first */
            ret = PyLong_FromLong(-1);
        else if ((short)HRESULT_CODE(hr) > 0)
            /* pidl2 comes first */
            ret = PyLong_FromLong(1);
        else
            /* the two pidls are equal */
            ret = PyLong_FromLong(0);
    }
    PyObject_FreePIDL(pidl1);
    PyObject_FreePIDL(pidl2);
    return ret;
}

// @pymethod <o PyIShellView>|PyIShellFolder|CreateViewObject|Creates a view object for a shell folder.
PyObject *PyIShellFolder::CreateViewObject(PyObject *self, PyObject *args)
{
    IShellFolder *pISF = GetI(self);
    if (pISF == NULL)
        return NULL;
    // @pyparm HWND|hwndOwner||Parent window for a custom folder view, or 0
    // @pyparm <o PyIID>|riid||IID of the desired interface, usually IID_IShellView
    PyObject *obriid;
    HWND hwndOwner;
    PyObject *obhwndOwner;
    IID riid;
    void *out;
    if (!PyArg_ParseTuple(args, "OO:CreateViewObject", &obhwndOwner, &obriid))
        return NULL;
    if (!PyWinObject_AsHANDLE(obhwndOwner, (HANDLE *)&hwndOwner))
        return NULL;
    if (!PyWinObject_AsIID(obriid, &riid))
        return NULL;
    HRESULT hr;
    PY_INTERFACE_PRECALL;
    hr = pISF->CreateViewObject(hwndOwner, riid, &out);

    PY_INTERFACE_POSTCALL;

    if (FAILED(hr))
        return PyCom_BuildPyException(hr, pISF, IID_IShellFolder);

    return PyCom_PyObjectFromIUnknown((IUnknown *)out, riid, FALSE);
}

// @pymethod int|PyIShellFolder|GetAttributesOf|Queries attributes of items within the shell folder
// @rdesc The requested attributes are only returned if they are common to all of the specified items
PyObject *PyIShellFolder::GetAttributesOf(PyObject *self, PyObject *args)
{
    IShellFolder *pISF = GetI(self);
    if (pISF == NULL)
        return NULL;
    // @pyparm (<o PyIDL>,...)|pidl||A sequence of single-level pidls identifying items directly contained by the folder
    // @pyparm int|rgfInOut||Combination of shellcon.SFGAO_* constants
    PyObject *obpidl;
    UINT cidl;
    LPCITEMIDLIST *pidl;
    ULONG rgfInOut;
    if (!PyArg_ParseTuple(args, "Ol:GetAttributesOf", &obpidl, &rgfInOut))
        return NULL;

    if (!PyObject_AsPIDLArray(obpidl, &cidl, &pidl))
        return NULL;
    HRESULT hr;
    PY_INTERFACE_PRECALL;
    hr = pISF->GetAttributesOf(cidl, pidl, &rgfInOut);
    PyObject_FreePIDLArray(cidl, pidl);

    PY_INTERFACE_POSTCALL;

    if (FAILED(hr))
        return PyCom_BuildPyException(hr, pISF, IID_IShellFolder);
    return PyLong_FromLong(rgfInOut);
}

// @pymethod int, <o PyIUnknown>|PyIShellFolder|GetUIObjectOf|Creates an interface to one or more items in a shell
// folder
// @rdesc Returns the Reserved parameter and the requested interface
PyObject *PyIShellFolder::GetUIObjectOf(PyObject *self, PyObject *args)
{
    IShellFolder *pISF = GetI(self);
    if (pISF == NULL)
        return NULL;
    // @pyparm <o PyHANDLE>|hwndOwner||Specifies a window in which to display any required dialogs or errors, can be 0
    // @pyparm (<o PyIDL>,...)|pidl||A sequence of single-level pidls identifying items in the folder
    // @pyparm <o PyIID>|riid||The interface to create, one of IID_IContextMenu, IID_IContextMenu2, IID_IDataObject,
    // IID_IDropTarget, IID_IExtractIcon, IID_IQueryInfo
    // @pyparm int|Reserved|0|Reserved, use 0 if passed in
    // @pyparm <o PyIID>|iidout|riid|The interface to return.  Can be used in the case where there is not a
    //  python wrapper for the desired interface.  You must make certain that the interface identified by riid
    //  actually supports the iidout interface, or Bad Things Will Happen.
    //  It should always be safe to return <o PyIUnknown>, which is the base for all interfaces.
    PyObject *obpidl;
    PyObject *obriid;
    PyObject *obiidout = NULL;
    PyObject *obhwndOwner;
    HWND hwndOwner;
    UINT cidl;
    LPCITEMIDLIST *pidl;
    IID riid, iidout;
    UINT rgfInOut = 0;
    void *out;
    if (!PyArg_ParseTuple(args, "OOO|lO:GetUIObjectOf", &obhwndOwner, &obpidl, &obriid, &rgfInOut, &obiidout))
        return NULL;
    if (!PyWinObject_AsHANDLE(obhwndOwner, (HANDLE *)&hwndOwner))
        return NULL;
    if (!PyWinObject_AsIID(obriid, &riid))
        return NULL;
    if (obiidout == NULL)
        iidout = riid;
    else if (!PyWinObject_AsIID(obiidout, &iidout))
        return NULL;
    if (!PyObject_AsPIDLArray(obpidl, &cidl, &pidl))
        return NULL;

    HRESULT hr;
    PY_INTERFACE_PRECALL;
    hr = pISF->GetUIObjectOf(hwndOwner, cidl, pidl, riid, &rgfInOut, &out);
    PyObject_FreePIDLArray(cidl, pidl);

    PY_INTERFACE_POSTCALL;

    if (FAILED(hr))
        return PyCom_BuildPyException(hr, pISF, IID_IShellFolder);
    return Py_BuildValue("lN", rgfInOut, PyCom_PyObjectFromIUnknown((IUnknown *)out, iidout, FALSE));
}

// @pymethod str|PyIShellFolder|GetDisplayNameOf|Returns the display name of an item within this shell folder
PyObject *PyIShellFolder::GetDisplayNameOf(PyObject *self, PyObject *args)
{
    IShellFolder *pISF = GetI(self);
    if (pISF == NULL)
        return NULL;
    // @pyparm <o PyIDL>|pidl||PIDL that identifies the item relative to the parent folder
    // @pyparm int|uFlags||Combination of shellcon.SHGDN_* flags
    PyObject *obpidl;
    ITEMIDLIST *pidl;
    DWORD uFlags;
    if (!PyArg_ParseTuple(args, "Ol:GetDisplayNameOf", &obpidl, &uFlags))
        return NULL;
    if (!PyObject_AsPIDL(obpidl, &pidl))
        return NULL;
    HRESULT hr;
    STRRET out;
    PY_INTERFACE_PRECALL;
    hr = pISF->GetDisplayNameOf(pidl, uFlags, &out);
    PY_INTERFACE_POSTCALL;
    PyObject *ret;
    if (FAILED(hr))
        ret = PyCom_BuildPyException(hr, pISF, IID_IShellFolder);
    else
        ret = PyObject_FromSTRRET(&out, pidl, TRUE);
    PyObject_FreePIDL(pidl);
    return ret;
}

// @pymethod <o PyIDL>|PyIShellFolder|SetNameOf|Sets the display name of an item and changes its PIDL
// @rdesc Returns the new PIDL for item
PyObject *PyIShellFolder::SetNameOf(PyObject *self, PyObject *args)
{
    IShellFolder *pISF = GetI(self);
    if (pISF == NULL)
        return NULL;
    // @pyparm HWND|hwndOwner||Window in which to display any message boxes or dialogs, can be 0
    // @pyparm <o PyIDL>|pidl||PIDL that identifies the item relative to the parent folder
    // @pyparm str|Name||New name for the item
    // @pyparm int|Flags||Combination of shellcon.SHGDM_* values
    PyObject *obpidl = NULL, *ret = NULL;
    PyObject *obName = NULL;
    PyObject *obhwndOwner;
    HWND hwndOwner;
    ITEMIDLIST *pidl = NULL;
    ITEMIDLIST *pidlRet = NULL;
    TmpWCHAR Name;
    DWORD flags;

    if (!PyArg_ParseTuple(args, "OOOl:SetNameOf", &obhwndOwner, &obpidl, &obName, &flags))
        return NULL;
    if (!PyWinObject_AsHANDLE(obhwndOwner, (HANDLE *)&hwndOwner))
        return NULL;
    if (!PyWinObject_AsWCHAR(obName, &Name))
        return NULL;
    if (!PyObject_AsPIDL(obpidl, &pidl))
        return NULL;

    HRESULT hr;
    PY_INTERFACE_PRECALL;
    hr = pISF->SetNameOf(hwndOwner, pidl, Name, (SHGDNF)flags, &pidlRet);
    PY_INTERFACE_POSTCALL;
    PyObject_FreePIDL(pidl);
    if (FAILED(hr))
        return PyCom_BuildPyException(hr, pISF, IID_IShellFolder);
    return PyObject_FromPIDL(pidlRet, TRUE);
}

// @object PyIShellFolder|Interface that represents an Explorer folder
static struct PyMethodDef PyIShellFolder_methods[] = {
    {"ParseDisplayName", PyIShellFolder::ParseDisplayName,
     1},  // @pymeth ParseDisplayName|Returns the PIDL of an item in a shell folder
    {"EnumObjects", PyIShellFolder::EnumObjects,
     1},  // @pymeth EnumObjects|Creates an enumerator to list the contents of the shell folder
    {"BindToObject", PyIShellFolder::BindToObject,
     1},  // @pymeth BindToObject|Returns an IShellFolder interface for a subfolder
    {"BindToStorage", PyIShellFolder::BindToStorage,
     1},  // @pymeth BindToStorage|Returns an interface to a storage object in a shell folder
    {"CompareIDs", PyIShellFolder::CompareIDs,
     1},  // @pymeth CompareIDs|Determines the sorting order of 2 items in shell folder
    {"CreateViewObject", PyIShellFolder::CreateViewObject,
     1},  // @pymeth CreateViewObject|Creates a view object for a shell folder.
    {"GetAttributesOf", PyIShellFolder::GetAttributesOf,
     1},  // @pymeth GetAttributesOf|Queries attributes of items within the shell folder
    {"GetUIObjectOf", PyIShellFolder::GetUIObjectOf,
     1},  // @pymeth GetUIObjectOf|Creates an interface to one or more items in a shell folder
    {"GetDisplayNameOf", PyIShellFolder::GetDisplayNameOf,
     1},  // @pymeth GetDisplayNameOf|Returns the display name of an item within this shell folder
    {"SetNameOf", PyIShellFolder::SetNameOf,
     1},  // @pymeth SetNameOf|Sets the display name of an item and changes its PIDL
    {NULL}};

// @pymeth __iter__|Enumerates all objects in this folder.
PyComEnumProviderTypeObject PyIShellFolder::type("PyIShellFolder", &PyIUnknown::type, sizeof(PyIShellFolder),
                                                 PyIShellFolder_methods, GET_PYCOM_CTOR(PyIShellFolder), "EnumObjects");
// ---------------------------------------------------
//
// Gateway Implementation
STDMETHODIMP PyGShellFolder::ParseDisplayName(
    /* [unique][in] */ HWND hwndOwner,
    /* [unique][in] */ LPBC pbcReserved,
    /* [unique][in] */ LPOLESTR lpszDisplayName,
    /* [out] */ ULONG __RPC_FAR *pchEaten,
    /* [out] */ LPITEMIDLIST *ppidl,
    /* [out] */ ULONG __RPC_FAR *pdwAttributes)
{
    PY_GATEWAY_METHOD;
    *ppidl = NULL;
    PyObject *obpbcReserved;
    PyObject *oblpszDisplayName;
    obpbcReserved = PyCom_PyObjectFromIUnknown(pbcReserved, IID_IBindCtx, TRUE);
    oblpszDisplayName = MakeOLECHARToObj(lpszDisplayName);
    PyObject *result;
    HRESULT hr = InvokeViaPolicy("ParseDisplayName", &result, "NOOl", PyWinLong_FromHANDLE(hwndOwner), obpbcReserved,
                                 oblpszDisplayName, pdwAttributes ? *pdwAttributes : 0);
    Py_XDECREF(obpbcReserved);
    Py_XDECREF(oblpszDisplayName);
    if (FAILED(hr))
        return hr;
    // Process the Python results, and convert back to the real params
    PyObject *obppidl;
    ULONG chEaten, dwAttributes;
    BOOL bPythonIsHappy = TRUE;
    if (!PyTuple_Check(result)) {
        PyErr_Format(PyExc_TypeError, "ParseDisplayName must return a tuple of (int, pidl, attr) - got '%s'",
                     result->ob_type->tp_name);
        bPythonIsHappy = FALSE;
    }
    if (bPythonIsHappy && !PyArg_ParseTuple(result, "lOl", &chEaten, &obppidl, &dwAttributes))
        bPythonIsHappy = FALSE;
    if (bPythonIsHappy && !PyObject_AsPIDL(obppidl, ppidl))
        bPythonIsHappy = FALSE;
    if (!bPythonIsHappy)
        hr = PyCom_SetAndLogCOMErrorFromPyException("ParseDisplayName", IID_IShellFolder);
    if (pchEaten)
        *pchEaten = chEaten;
    if (pdwAttributes)
        *pdwAttributes = dwAttributes;
    Py_DECREF(result);
    return hr;
}

STDMETHODIMP PyGShellFolder::EnumObjects(
    /* [unique][in] */ HWND hwndOwner,
    /* [unique][in] */ DWORD grfFlags,
    /* [out] */ IEnumIDList __RPC_FAR **ppeidl)
{
    PY_GATEWAY_METHOD;
    PyObject *result;
    HRESULT hr = InvokeViaPolicy("EnumObjects", &result, "Nl", PyWinLong_FromHANDLE(hwndOwner), grfFlags);
    if (FAILED(hr))
        return hr;
    // Process the Python results, and convert back to the real params
    if (result == Py_None) {
        hr = S_FALSE;
        *ppeidl = NULL;
    }
    else {
        PyCom_InterfaceFromPyInstanceOrObject(result, IID_IEnumIDList, (void **)ppeidl, FALSE /* bNoneOK */);
        hr = PyCom_SetAndLogCOMErrorFromPyException("EnumObjects", IID_IShellFolder);
    }
    Py_DECREF(result);
    return hr;
}

STDMETHODIMP PyGShellFolder::BindToObject(
    /* [unique][in] */ LPCITEMIDLIST pidl,
    /* [unique][in] */ LPBC pbcReserved,
    /* [unique][in] */ REFIID riid,
    /* [out] */ void **out)
{
    static const char *szMethodName = "BindToObject";
    PY_GATEWAY_METHOD;
    PyObject *obpidl;
    PyObject *obpbcReserved;
    PyObject *obriid;
    obpidl = PyObject_FromPIDL(pidl, FALSE);
    obpbcReserved = PyCom_PyObjectFromIUnknown(pbcReserved, IID_IBindCtx, TRUE);
    obriid = PyWinObject_FromIID(riid);
    PyObject *result;
    HRESULT hr = InvokeViaPolicy(szMethodName, &result, "OOO", obpidl, obpbcReserved, obriid);
    Py_XDECREF(obpidl);
    Py_XDECREF(obpbcReserved);
    Py_XDECREF(obriid);
    if (FAILED(hr))
        return hr;
    // Process the Python results, and convert back to the real params
    PyCom_InterfaceFromPyInstanceOrObject(result, riid, out, FALSE /* bNoneOK */);
    hr = PyCom_SetAndLogCOMErrorFromPyException(szMethodName, IID_IShellFolder);
    Py_DECREF(result);
    return hr;
}

STDMETHODIMP PyGShellFolder::BindToStorage(
    /* [unique][in] */ LPCITEMIDLIST pidl,
    /* [unique][in] */ LPBC pbcReserved,
    /* [unique][in] */ REFIID riid,
    /* [out] */ void **ppRet)
{
    static const char *szMethodName = "BindToStorage";
    PY_GATEWAY_METHOD;
    PyObject *obpidl;
    PyObject *obpbcReserved;
    PyObject *obriid;
    obpidl = PyObject_FromPIDL(pidl, FALSE);
    obpbcReserved = PyCom_PyObjectFromIUnknown(pbcReserved, IID_IBindCtx, TRUE);
    obriid = PyWinObject_FromIID(riid);
    PyObject *result;
    HRESULT hr = InvokeViaPolicy(szMethodName, &result, "OOO", obpidl, obpbcReserved, obriid);
    Py_XDECREF(obpidl);
    Py_XDECREF(obpbcReserved);
    Py_XDECREF(obriid);
    if (FAILED(hr))
        return hr;
    // Process the Python results, and convert back to the real params
    PyCom_InterfaceFromPyInstanceOrObject(result, riid, ppRet, FALSE /* bNoneOK */);
    hr = PyCom_SetAndLogCOMErrorFromPyException(szMethodName, IID_IShellFolder);
    Py_DECREF(result);
    return hr;
}

STDMETHODIMP PyGShellFolder::CompareIDs(
    /* [unique][in] */ LPARAM lparam,
    /* [unique][in] */ LPCITEMIDLIST pidl1,
    /* [unique][in] */ LPCITEMIDLIST pidl2)
{
    PY_GATEWAY_METHOD;
    PyObject *obparam = PyWinObject_FromPARAM(lparam);
    PyObject *obpidl1 = PyObject_FromPIDL(pidl1, FALSE);
    PyObject *obpidl2 = PyObject_FromPIDL(pidl2, FALSE);
    PyObject *result;
    HRESULT hr = InvokeViaPolicy("CompareIDs", &result, "NNN", obparam, obpidl1, obpidl2);
    if (FAILED(hr))
        return hr;
    if (PyLong_Check(result))
        hr = MAKE_HRESULT(SEVERITY_SUCCESS, 0, PyLong_AsLong(result));
    Py_DECREF(result);
    return hr;
}

STDMETHODIMP PyGShellFolder::CreateViewObject(
    /* [unique][in] */ HWND hwndOwner,
    /* [unique][in] */ REFIID riid,
    /* [out] */ void **ppRet)
{
    static const char *szMethodName = "CreateViewObject";
    PY_GATEWAY_METHOD;
    PyObject *obriid;
    obriid = PyWinObject_FromIID(riid);
    PyObject *result;
    HRESULT hr = InvokeViaPolicy(szMethodName, &result, "NO", PyWinLong_FromHANDLE(hwndOwner), obriid);
    Py_XDECREF(obriid);
    if (FAILED(hr))
        return hr;
    // Process the Python results, and convert back to the real params
    PyCom_InterfaceFromPyInstanceOrObject(result, riid, ppRet, FALSE /* bNoneOK */);
    hr = PyCom_SetAndLogCOMErrorFromPyException(szMethodName, IID_IShellFolder);
    Py_DECREF(result);
    return hr;
}

STDMETHODIMP PyGShellFolder::GetAttributesOf(
    /* [unique][in] */ UINT cidl,
    /* [unique][in] */ LPCITEMIDLIST *apidl,
    /* [unique][in][out] */ ULONG __RPC_FAR *rgfInOut)
{
    PY_GATEWAY_METHOD;
    PyObject *obpidl;
    obpidl = PyObject_FromPIDLArray(cidl, apidl);
    PyObject *result;
    HRESULT hr = InvokeViaPolicy("GetAttributesOf", &result, "Ol", obpidl, rgfInOut ? *rgfInOut : 0);
    Py_XDECREF(obpidl);
    if (FAILED(hr))
        return hr;
    // Process the Python results, and convert back to the real params
    if (rgfInOut && PyLong_Check(result))
        *rgfInOut = PyLong_AsLong(result);
    hr = PyCom_SetAndLogCOMErrorFromPyException("GetAttributesOf", IID_IShellFolder);
    Py_DECREF(result);
    return hr;
}

STDMETHODIMP PyGShellFolder::GetUIObjectOf(
    /* [unique][in] */ HWND hwndOwner,
    /* [unique][in] */ UINT cidl,
    /* [unique][in] */ LPCITEMIDLIST *apidl,
    /* [unique][in] */ REFIID riid,
    /* [unique][in][out] */ UINT *rgfInOut,
    /* [out] */ void **ppRet)
{
    static const char *szMethodName = "GetUIObjectOf";
    PY_GATEWAY_METHOD;
    PyObject *obpidl;
    PyObject *obriid;
    obpidl = PyObject_FromPIDLArray(cidl, apidl);
    obriid = PyWinObject_FromIID(riid);
    PyObject *result;
    HRESULT hr = InvokeViaPolicy(szMethodName, &result, "NOOl", PyWinLong_FromHANDLE(hwndOwner), obpidl, obriid,
                                 rgfInOut ? *rgfInOut : 0);
    Py_XDECREF(obpidl);
    Py_XDECREF(obriid);
    if (FAILED(hr))
        return hr;
    // Process the Python results, and convert back to the real params
    // the 'inout' param appears unused - allow either.
    if (PyTuple_Check(result)) {
        PyObject *obout;
        UINT inout;
        if (!PyArg_ParseTuple(result, "lO", &inout, &obout))
            hr = PyCom_SetAndLogCOMErrorFromPyException(szMethodName, IID_IShellFolder);
        else {
            if (rgfInOut)
                *rgfInOut = inout;
            if (!PyCom_InterfaceFromPyInstanceOrObject(obout, riid, ppRet, FALSE /* bNoneOK */))
                hr = PyCom_SetAndLogCOMErrorFromPyException(szMethodName, IID_IShellFolder);
        }
    }
    else {
        if (!PyCom_InterfaceFromPyInstanceOrObject(result, riid, ppRet, FALSE /* bNoneOK */))
            hr = PyCom_SetAndLogCOMErrorFromPyException(szMethodName, IID_IShellFolder);
    }
    Py_DECREF(result);
    return hr;
}

STDMETHODIMP PyGShellFolder::GetDisplayNameOf(
    /* [unique][in] */ LPCITEMIDLIST pidl,
    /* [unique][in] */ SHGDNF uFlags,
    /* [out] */ STRRET __RPC_FAR *out)
{
    static const char *szMethodName = "GetDisplayNameOf";
    if (!out)
        return E_POINTER;
    PY_GATEWAY_METHOD;
    PyObject *obpidl;
    obpidl = PyObject_FromPIDL(pidl, FALSE);
    PyObject *result;
    HRESULT hr = InvokeViaPolicy(szMethodName, &result, "Ol", obpidl, uFlags);
    Py_XDECREF(obpidl);
    if (FAILED(hr))
        return hr;
    // Process the Python results, and convert back to the real params
    out->uType = STRRET_WSTR;
    if (!PyWinObject_AsTaskAllocatedWCHAR(result, &out->pOleStr))
        hr = PyCom_SetAndLogCOMErrorFromPyException(szMethodName, IID_IShellFolder);
    Py_DECREF(result);
    return hr;
}

STDMETHODIMP PyGShellFolder::SetNameOf(
    /* [in] */ HWND hwnd,
    /* [in] */ LPCITEMIDLIST pidl,
    /* [string][in] */ LPCOLESTR pszName,
    /* [in] */ SHGDNF uFlags,
    /* [out] */ LPITEMIDLIST *ppidlOut)
{
    static const char *szMethodName = "SetNameOf";
    PY_GATEWAY_METHOD;
    if (ppidlOut)
        (*ppidlOut) = NULL;
    PyObject *obpidl;
    PyObject *oblpszName;
    obpidl = PyObject_FromPIDL(pidl, FALSE);
    oblpszName = MakeOLECHARToObj(pszName);
    PyObject *result;
    HRESULT hr = InvokeViaPolicy(szMethodName, &result, "NOOl", PyWinLong_FromHANDLE(hwnd), obpidl, oblpszName, uFlags);
    Py_XDECREF(obpidl);
    Py_XDECREF(oblpszName);
    if (FAILED(hr))
        return hr;
    if (ppidlOut) {
        PyObject_AsPIDL(result, ppidlOut, FALSE);
        hr = PyCom_SetAndLogCOMErrorFromPyException(szMethodName, IID_IShellFolder);
    }
    Py_DECREF(result);
    return hr;
}
