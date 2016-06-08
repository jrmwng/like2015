using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Microsoft.VisualStudio;
using Microsoft.VisualStudio.Shell.Interop;

using System.Diagnostics;
using System.Runtime.InteropServices;

using VSITEMID = Microsoft.VisualStudio.VSConstants.VSITEMID;

namespace VSIXProject1
{
    class VsItem
    {
        private readonly IVsHierarchy m_vsHierarchy;
        private readonly VSITEMID m_vsItemID;

        private readonly VsItem m_vsParent;

        public VsItem(IVsProject vsProject)
        {
            m_vsHierarchy = vsProject as IVsHierarchy;
            Debug.Assert(object.ReferenceEquals(m_vsHierarchy, null) == object.ReferenceEquals(vsProject, null));

            m_vsItemID = VSITEMID.Root;
            m_vsParent = null;
        }
        private VsItem(IVsHierarchy vsHierarchy, VSITEMID vsItemID, VsItem vsParent)
        {
            m_vsHierarchy = vsHierarchy;
            m_vsItemID = vsItemID;
            m_vsParent = vsParent;
        }

        public object GetProperty(__VSHPROPID vsPropID)
        {
            object objOut = null;
            m_vsHierarchy.GetProperty((uint)m_vsItemID, (int)vsPropID, out objOut);
            return objOut;
        }
        public object GetProperty(__VSHPROPID2 vsPropID)
        {
            object objOut = null;
            m_vsHierarchy.GetProperty((uint)m_vsItemID, (int)vsPropID, out objOut);
            return objOut;
        }
        public object GetProperty(__VSHPROPID4 vsPropID)
        {
            object objOut = null;
            m_vsHierarchy.GetProperty((uint)m_vsItemID, (int)vsPropID, out objOut);
            return objOut;
        }

        public VSITEMID ID
        {
            get
            {
                return m_vsItemID;
            }
        }
        public VSITEMID ParentID
        {
            get
            {
                return object.ReferenceEquals(m_vsParent, null) ? VSITEMID.Nil : m_vsParent.ID;
            }
        }
        public VsItem ParentItem
        {
            get
            {
                return m_vsParent;
            }
        }

        public IEnumerable<VSITEMID> ChildrenID
        {
            get
            {
                IVsHierarchy vsHierarchy = m_vsHierarchy;
                VSITEMID vsItemID = m_vsItemID;
                object objChildID;
                uint uChildID;

                for (objChildID = null, vsHierarchy.GetProperty((uint)vsItemID, (int)__VSHPROPID.VSHPROPID_FirstChild, out objChildID); !object.ReferenceEquals(objChildID, null); objChildID = null, vsHierarchy.GetProperty(uChildID, (int)__VSHPROPID.VSHPROPID_NextSibling, out objChildID))
                {
                    uChildID = (uint)(int)objChildID;

                    VSITEMID vsChildID = (VSITEMID)uChildID;

                    if (vsChildID != VSITEMID.Nil)
                    {
                        yield return vsChildID;
                    }
                }
            }
        }
        public IEnumerable<VsItem> ChildrenItem
        {
            get
            {
                IVsHierarchy vsHierarchy = m_vsHierarchy;
                VSITEMID vsItemID = m_vsItemID;
                object objChildID;
                uint uChildID;

                for (objChildID = null, vsHierarchy.GetProperty((uint)vsItemID, (int)__VSHPROPID.VSHPROPID_FirstChild, out objChildID); !object.ReferenceEquals(objChildID, null); objChildID = null, vsHierarchy.GetProperty(uChildID, (int)__VSHPROPID.VSHPROPID_NextSibling, out objChildID))
                {
                    uChildID = (uint)(int)objChildID;

                    VSITEMID vsChildID = (VSITEMID)uChildID;

                    if (vsChildID != VSITEMID.Nil)
                    {
                        yield return new VsItem(vsHierarchy, vsChildID, this);
                    }
                }
            }
        }

        public IEnumerable<VSITEMID> DescendentsID
        {
            get
            {
                IVsHierarchy vsHierarchy = m_vsHierarchy;
                VSITEMID vsItemID = m_vsItemID;
                object objChildID;
                uint uChildID;

                for (objChildID = null, vsHierarchy.GetProperty((uint)vsItemID, (int)__VSHPROPID.VSHPROPID_FirstChild, out objChildID); !object.ReferenceEquals(objChildID, null); objChildID = null, vsHierarchy.GetProperty(uChildID, (int)__VSHPROPID.VSHPROPID_NextSibling, out objChildID))
                {
                    uChildID = (uint)(int)objChildID;

                    VSITEMID vsChildID = (VSITEMID)uChildID;

                    if (vsChildID != VSITEMID.Nil)
                    {
                        yield return vsChildID;

                        VsItem vsChildItem = new VsItem(vsHierarchy, vsChildID, this);

                        foreach (VSITEMID vsDescendentID in vsChildItem.DescendentsID)
                        {
                            yield return vsDescendentID;
                        }
                    }
                }
            }
        }
        public IEnumerable<VsItem> DescendentsItem
        {
            get
            {
                IVsHierarchy vsHierarchy = m_vsHierarchy;
                VSITEMID vsItemID = m_vsItemID;
                object objChildID;
                uint uChildID;

                for (objChildID = null, vsHierarchy.GetProperty((uint)vsItemID, (int)__VSHPROPID.VSHPROPID_FirstChild, out objChildID); !object.ReferenceEquals(objChildID, null); objChildID = null, vsHierarchy.GetProperty(uChildID, (int)__VSHPROPID.VSHPROPID_NextSibling, out objChildID))
                {
                    uChildID = (uint)(int)objChildID;

                    VSITEMID vsChildID = (VSITEMID)uChildID;

                    if (vsChildID != VSITEMID.Nil)
                    {
                        VsItem vsChildItem = new VsItem(vsHierarchy, vsChildID, this);

                        yield return vsChildItem;

                        foreach (VsItem vsDescedent in vsChildItem.DescendentsItem)
                        {
                            yield return vsDescedent;
                        }
                    }
                }
            }
        }

        public IEnumerable<VSITEMID> AncestorsID
        {
            get
            {
                for (VsItem vsAncestorItem = this.ParentItem; !object.ReferenceEquals(vsAncestorItem, null); vsAncestorItem = vsAncestorItem.ParentItem)
                {
                    yield return vsAncestorItem.ID;
                }
            }
        }
        public IEnumerable<VsItem> AncestorsItem
        {
            get
            {
                for (VsItem vsAncestorItem = this.ParentItem; !object.ReferenceEquals(vsAncestorItem, null); vsAncestorItem = vsAncestorItem.ParentItem)
                {
                    yield return vsAncestorItem;
                }
            }
        }

        //
        // Summary:
        //     BOOL. [optional] By default, specifies whether "Deploy" should be initially selected
        //     in the solution cfg. Typically, "Deploy" is selected by if the project supports
        //     Microsoft.VisualStudio.Shell.Interop.IVsDeployableProjectCfg.
        //VSHPROPID_DefaultEnableDeployProjectCfg = -2064,
        public bool DefaultEnableDeployProjectCfg
        {
            get
            {
                return (bool)GetProperty(__VSHPROPID.VSHPROPID_DefaultEnableDeployProjectCfg);
            }
        }
        //
        // Summary:
        //     INT_PTR. Specifies the first property ID.
        //VSHPROPID_FIRST = -2064,
        public int FIRST
        {
            get
            {
                return (int)GetProperty(__VSHPROPID.VSHPROPID_FIRST);
            }
        }
        //
        // Summary:
        //     BOOL. [optional] By default, specifies whether "Build" should be initially selected
        //     in the solution cfg. Typically, "Build" is selected if the project supports Microsoft.VisualStudio.Shell.Interop.IVsBuildableProjectCfg.
        //VSHPROPID_DefaultEnableBuildProjectCfg = -2063,
        public bool DefaultEnableBuildProjectCfg
        {
            get
            {
                return (bool)GetProperty(__VSHPROPID.VSHPROPID_DefaultEnableBuildProjectCfg);
            }
        }
        //
        // Summary:
        //     BOOL. [optional] If true, then this hierarchy is not enumerated for FindInFiles
        //     and similar hierarchy enumerations. (Useful if enumerating is unacceptably slow
        //     or could cause a login dialog box to appear.)
        //VSHPROPID_HasEnumerationSideEffects = -2062,
        public bool HasEnumerationSideEffects
        {
            get
            {
                return (bool)GetProperty(__VSHPROPID.VSHPROPID_HasEnumerationSideEffects);
            }
        }
        //
        // Summary:
        //     I4. [optional] Value from the Microsoft.VisualStudio.Shell.Interop.VSDESIGNER_FUNCTIONVISIBILITY
        //     enumeration.
        //VSHPROPID_DesignerFunctionVisibility = -2061,
        public VSDESIGNER_FUNCTIONVISIBILITY DesignerFunctionVisibility
        {
            get
            {
                return (VSDESIGNER_FUNCTIONVISIBILITY)(int)GetProperty(__VSHPROPID.VSHPROPID_DesignerFunctionVisibility);
            }
        }
        //
        // Summary:
        //     I4. [optional] Value from the Microsoft.VisualStudio.Shell.Interop.VSDESIGNER_VARIABLENAMING
        //     enumeration.
        //VSHPROPID_DesignerVariableNaming = -2060,
        public VSDESIGNER_VARIABLENAMING DesignerVariableNaming
        {
            get
            {
                return (VSDESIGNER_VARIABLENAMING)(int)GetProperty(__VSHPROPID.VSHPROPID_DesignerVariableNaming);
            }
        }
        //
        // Summary:
        //     GUID. [optional] Identifies a project across solutions. Generated and set when
        //     the project is created. Retrieved when the project is opened.
        //VSHPROPID_ProjectIDGuid = -2059,
        public Guid ProjectIDGuid
        {
            get
            {
                return (Guid)GetProperty(__VSHPROPID.VSHPROPID_ProjectIDGuid);
            }
        }
        //
        // Summary:
        //     BOOL. [optional] If true, only the caption is shown in the UI instead of the
        //     full moniker.
        //VSHPROPID_ShowOnlyItemCaption = -2058,
        public bool ShowOnlyItemCaption
        {
            get
            {
                return (bool)GetProperty(__VSHPROPID.VSHPROPID_ShowOnlyItemCaption);
            }
        }
        //
        // Summary:
        //     BOOL. [optional] If true, this is a new, unsaved item (as obtained from File.New.File)
        //     and therefore, the moniker is a temporary name and the caption should be used
        //     in the UI instead.
        //VSHPROPID_IsNewUnsavedItem = -2057,
        public bool IsNewUnsavedItem
        {
            get
            {
                return (bool)GetProperty(__VSHPROPID.VSHPROPID_IsNewUnsavedItem);
            }
        }
        //
        // Summary:
        //     BOOL. [optional] If this is false or Microsoft.VisualStudio.VSConstants.E_NOTIMPL,
        //     the compiler disables modifications during run mode. (This is the most common
        //     expected behavior.) true enables edits. This property enables VSA scenarios.
        //VSHPROPID_AllowEditInRunMode = -2056,
        public bool AllowEditInRunMode
        {
            get
            {
                return (bool)GetProperty(__VSHPROPID.VSHPROPID_AllowEditInRunMode);
            }
        }
        //
        // Summary:
        //     BOOL. [optional] Used to filter the project when Visual Basic or C# calls the
        //     component picker for add-reference. Returns VARIANT_TRUE to show that the project
        //     is in the list.
        //VSHPROPID_ShowProjInSolutionPage = -2055,
        public bool ShowProjInSolutionPage
        {
            get
            {
                return (bool)GetProperty(__VSHPROPID.VSHPROPID_ShowProjInSolutionPage);
            }
        }
        //
        // Summary:
        //     GUID. [optional] Preferred SID of the (text editor) language service for the
        //     project.
        //VSHPROPID_PreferredLanguageSID = -2054,
        public Guid PreferredLanguageSID
        {
            get
            {
                return (Guid)GetProperty(__VSHPROPID.VSHPROPID_PreferredLanguageSID);
            }
        }
        //
        // Summary:
        //     BOOL. [optional] If true, do not prompt to save before building.
        //VSHPROPID_CanBuildFromMemory = -2053,
        public bool CanBuildFromMemory
        {
            get
            {
                return (bool)GetProperty(__VSHPROPID.VSHPROPID_CanBuildFromMemory);
            }
        }
        //
        // Summary:
        //     BOOL. [optional] If true, FindInFiles runs in the main thread.
        //VSHPROPID_IsFindInFilesForegroundOnly = -2052,
        public bool IsFindInFilesForegroundOnly
        {
            get
            {
                return (bool)GetProperty(__VSHPROPID.VSHPROPID_IsFindInFilesForegroundOnly);
            }
        }
        //
        // Summary:
        //     BOOL. [optional] Specifies that the item is not subject to search-and-replace
        //     through Find/Replace in Files. Should be true for the project file itself and
        //     for project references.
        //VSHPROPID_IsNonSearchable = -2051,
        public bool IsNonSearchable
        {
            get
            {
                return (bool)GetProperty(__VSHPROPID.VSHPROPID_IsNonSearchable);
            }
        }
        //
        // Summary:
        //     BSTR. [optional] String that represents the item's folder-based namespace: rootnamespace.folder.subfolder.
        //VSHPROPID_DefaultNamespace = -2049,
        public string DefaultNamespace
        {
            get
            {
                return GetProperty(__VSHPROPID.VSHPROPID_DefaultNamespace) as string;
            }
        }
        //
        // Summary:
        //     I4. [optional] Use the Microsoft.VisualStudio.Shell.Interop.VSOVERLAYICON enumeration.
        //     Overlay for the item's main icon.
        //VSHPROPID_OverlayIconIndex = -2048,
        public VSOVERLAYICON OverlayIconIndex
        {
            get
            {
                return (VSOVERLAYICON)(Int32)GetProperty(__VSHPROPID.VSHPROPID_OverlayIconIndex);
            }
        }
        //
        // Summary:
        //     BSTR. [optional] Non-localize string that represents the subtype of the item.
        //     It is up to each package to agree on the meaning of this string.
        //VSHPROPID_ItemSubType = -2047,
        public string ItemSubType
        {
            get
            {
                return GetProperty(__VSHPROPID.VSHPROPID_ItemSubType) as string;
            }
        }
        //
        // Summary:
        //     BSTR. [optional] Non-localize string that represents storage type. Same as the
        //     string that's used in the VSDIR file to differentiate between different location
        //     MRU lists.
        //VSHPROPID_StorageType = -2046,
        public string StorageType
        {
            get
            {
                return GetProperty(__VSHPROPID.VSHPROPID_StorageType) as string;
            }
        }
        //
        // Summary:
        //     BOOL. [optional] Specifies whether project storage is local. Microsoft.VisualStudio.Shell.Interop.IPersistFileFormat
        //     is used for saving the project whether it is a local "file" or not.
        //VSHPROPID_IsNonLocalStorage = -2045,
        public int IsNonLocalStorage
        {
            get
            {
                return (int)GetProperty(__VSHPROPID.VSHPROPID_IsNonLocalStorage);
            }
        }
        //
        // Summary:
        //     BOOL. [optional] Specifies that the item is not considered a member of the hierarchy.
        //     Required if the hierarchy supports (supersetted) views of its contents—that is,
        //     if ShowAllFiles displays files in directories that are not members of the project.
        //VSHPROPID_IsNonMemberItem = -2044,
        public bool IsNonMemberItem
        {
            get
            {
                return (bool)GetProperty(__VSHPROPID.VSHPROPID_IsNonMemberItem);
            }
        }
        //
        // Summary:
        //     BOOL. [optional] Specifies that an item is not displayed in current UIHierarchyWindow
        //     view. Required if the hierarchy supports multiple (subsetted) views of its contents.
        //VSHPROPID_IsHiddenItem = -2043,
        public bool IsHiddenItem
        {
            get
            {
                return (bool)GetProperty(__VSHPROPID.VSHPROPID_IsHiddenItem);
            }
        }
        //
        // Summary:
        //     INT_PTR. [optional] Similar to NextSibling but only walks items to be displayed
        //     in UIHierarchyWindow. Required if the hierarchy supports multiple (subsetted
        //     or supersetted) views of its contents.
        //VSHPROPID_NextVisibleSibling = -2042,
        public VSITEMID NextVisibleSibling
        {
            get
            {
                return (VSITEMID)(int)GetProperty(__VSHPROPID.VSHPROPID_NextVisibleSibling);
            }
        }
        //
        // Summary:
        //     INT_PTR. [optional] Similar to FirstChild but only walks items to be displayed
        //     in UIHierarchyWindow. Required if the hierarchy supports multiple (subsetted
        //     or supersetted) views of its contents.
        //VSHPROPID_FirstVisibleChild = -2041,
        public VSITEMID FirstVisibleChild
        {
            get
            {
                return (VSITEMID)(int)GetProperty(__VSHPROPID.VSHPROPID_FirstVisibleChild);
            }
        }
        //
        // Summary:
        //     UNK. [optional] Returns an Microsoft.VisualStudio.Shell.Interop.IVsProjectStartupServices*
        //     to add services to be started on next project load (for Microsoft.VisualStudio.VSConstants.VSITEMID_ROOT
        //     only).
        //VSHPROPID_StartupServices = -2040,
        public IVsProjectStartupServices StartupServices
        {
            get
            {
                return GetProperty(__VSHPROPID.VSHPROPID_StartupServices) as IVsProjectStartupServices;
            }
        }
        //
        // Summary:
        //     BSTR. [optional] Owner key string that identifies the GUID of the owning project.
        //VSHPROPID_OwnerKey = -2038,
        public string OwnerKey
        {
            get
            {
                return GetProperty(__VSHPROPID.VSHPROPID_OwnerKey) as string;
            }
        }
        //
        // Summary:
        //     UNK. [optional] Microsoft.VisualStudio.Shell.Interop.IVsHierarchy implant for
        //     this hierarchy.
        //VSHPROPID_ImplantHierarchy = -2037,
        public IVsHierarchy ImplantHierarchy
        {
            get
            {
                return GetProperty(__VSHPROPID.VSHPROPID_ImplantHierarchy) as IVsHierarchy;
            }
        }
        //
        // Summary:
        //     OBSOLETE. Use Microsoft.VisualStudio.Shell.Interop.IVsGetCfgProvider instead.
        //VSHPROPID_ConfigurationProvider = -2036,
        public object ConfigurationProvider
        {
            get
            {
                return GetProperty(__VSHPROPID.VSHPROPID_ConfigurationProvider);
            }
        }
        //
        // Summary:
        //     BOOL. Specifies whether the node is currently expanded in the UIHierarchyWindow.
        //     Starts as false.
        //VSHPROPID_Expanded = -2035,
        public bool Expanded
        {
            get
            {
                return (bool)GetProperty(__VSHPROPID.VSHPROPID_Expanded);
            }
        }
        //
        // Summary:
        //     INT_PTR. [optional] The document cookie (an abstract value) of the hierarchy
        //     item.
        //VSHPROPID_ItemDocCookie = -2034,
        public IntPtr ItemDocCookie
        {
            get
            {
                return (IntPtr)GetProperty(__VSHPROPID.VSHPROPID_ItemDocCookie);
            }
        }
        //
        // Summary:
        //     INT_PTR. The itemid of this hierarchy in its parent hierarchy.
        //VSHPROPID_ParentHierarchyItemid = -2033,
        public VSITEMID ParentHierarchyItemid
        {
            get
            {
                return (VSITEMID)(int)GetProperty(__VSHPROPID.VSHPROPID_ParentHierarchyItemid);
            }
        }
        //
        // Summary:
        //     UNK. Microsoft.VisualStudio.Shell.Interop.IVsHierarchy that owns this hierarchy.
        //VSHPROPID_ParentHierarchy = -2032,
        public IVsHierarchy ParentHierarchy
        {
            get
            {
                return GetProperty(__VSHPROPID.VSHPROPID_ParentHierarchy) as IVsHierarchy;
            }
        }
        //
        // Summary:
        //     [obsolete] Use Microsoft.VisualStudio.Shell.Interop.__VSHPROPID.VSHPROPID_HandlesOwnReload.
        //VSHPROPID_ReloadableProjectFile = -2031,
        public object ReloadableProjectFile
        {
            get
            {
                return GetProperty(__VSHPROPID.VSHPROPID_ReloadableProjectFile);
            }
        }
        //
        // Summary:
        //     BOOL. [optional] Project handles unload/reload itself. (Otherwise, the environment
        //     handles it.)
        //VSHPROPID_HandlesOwnReload = -2031,
        public int HandlesOwnReload
        {
            get
            {
                return (int)GetProperty(__VSHPROPID.VSHPROPID_HandlesOwnReload);
            }
        }
        //
        // Summary:
        //     [obsolete] Use Microsoft.VisualStudio.Shell.Interop.__VSHPROPID.VSHPROPID_TypeName.
        //VSHPROPID_ProjectType = -2030,
        public object ProjectType
        {
            get
            {
                return GetProperty(__VSHPROPID.VSHPROPID_ProjectType);
            }
        }
        //
        // Summary:
        //     BSTR. Displays a name to identify the type of node/hierarchy. (Used in the title
        //     bar.)
        //VSHPROPID_TypeName = -2030,
        public string TypeName
        {
            get
            {
                return GetProperty(__VSHPROPID.VSHPROPID_TypeName) as string;
            }
        }
        //
        // Summary:
        //     I4. State icon index. Use Microsoft.VisualStudio.Shell.Interop.VsStateIcon enumeration.
        //VSHPROPID_StateIconIndex = -2029,
        public int StateIconIndex
        {
            get
            {
                return (int)GetProperty(__VSHPROPID.VSHPROPID_StateIconIndex);
            }
        }
        //
        // Summary:
        //     DISP. [optional] Returns a custom EnvDTE.SelectedItem object for the given itemid.
        //VSHPROPID_ExtSelectedItem = -2028,
        public EnvDTE.SelectedItem ExtSelectedItem
        {
            get
            {
                return GetProperty(__VSHPROPID.VSHPROPID_ExtSelectedItem) as EnvDTE.SelectedItem;
            }
        }
        //
        // Summary:
        //     DISP. [optional] For ITEMID_ROOT, this is the ext.object of the project (Project).
        //VSHPROPID_ExtObject = -2027,
        public object ExtObject
        {
            get
            {
                return GetProperty(__VSHPROPID.VSHPROPID_ExtObject);
            }
        }
        //
        // Summary:
        //     BSTR. [optional] String displayed for the in-place editing node caption.
        //VSHPROPID_EditLabel = -2026,
        public string EditLabel
        {
            get
            {
                return GetProperty(__VSHPROPID.VSHPROPID_EditLabel) as string;
            }
        }
        //
        // Summary:
        //     UNK. [optional] Microsoft.VisualStudio.Shell.Interop.IVsUserContext for the project
        //     or item.
        //VSHPROPID_UserContext = -2023,
        public IVsUserContext UserContext
        {
            get
            {
                return GetProperty(__VSHPROPID.VSHPROPID_UserContext) as IVsUserContext;
            }
        }
        //
        // Summary:
        //     I4. [optional] Sort priority in UIHierarchyWindow. By default, standard projects
        //     have priority zero.
        //VSHPROPID_SortPriority = -2022,
        public int SortPriority
        {
            get
            {
                return (int)GetProperty(__VSHPROPID.VSHPROPID_SortPriority);
            }
        }
        //
        // Summary:
        //     BSTR. [optional] Full path of the project directory (for Microsoft.VisualStudio.VSConstants.VSITEMID_ROOT
        //     only).
        //VSHPROPID_ProjectDir = -2021,
        public string ProjectDir
        {
            get
            {
                return GetProperty(__VSHPROPID.VSHPROPID_ProjectDir) as string;
            }
        }
        //
        // Summary:
        //     I4. [optional] Returns an itemid for Microsoft.VisualStudio.Shell.Interop.SVsTrackSelectionEx.
        //VSHPROPID_AltItemid = -2020,
        public VSITEMID AltItemid
        {
            get
            {
                return (VSITEMID)(int)GetProperty(__VSHPROPID.VSHPROPID_AltItemid);
            }
        }
        //
        // Summary:
        //     UNK. [optional] Returns an Microsoft.VisualStudio.Shell.Interop.IVsHierarchy
        //     for Microsoft.VisualStudio.Shell.Interop.SVsTrackSelectionEx.
        //VSHPROPID_AltHierarchy = -2019,
        public IVsHierarchy AltHierarchy
        {
            get
            {
                return GetProperty(__VSHPROPID.VSHPROPID_AltHierarchy) as IVsHierarchy;
            }
        }
        //
        // Summary:
        //     DISP. [optional] Returns an IDispatch for property browsing.
        //VSHPROPID_BrowseObject = -2018,
        public DispatchWrapper BrowseObject
        {
            get
            {
                return new DispatchWrapper(GetProperty(__VSHPROPID.VSHPROPID_BrowseObject));
            }
        }
        //
        // Summary:
        //     UNK. [optional] Returns an Microsoft.VisualStudio.Shell.Interop.ISelectionContainer
        //     for use with property browsing.
        //VSHPROPID_SelContainer = -2017,
        public ISelectionContainer SelContainer
        {
            get
            {
                return GetProperty(__VSHPROPID.VSHPROPID_SelContainer) as ISelectionContainer;
            }
        }
        //
        // Summary:
        //     GUID for command bars (for root only).
        //VSHPROPID_CmdUIGuid = -2016,
        public Guid CmdUIGuid
        {
            get
            {
                return (Guid)GetProperty(__VSHPROPID.VSHPROPID_CmdUIGuid);
            }
        }
        //
        // Summary:
        //     I4. [optional] Index for OpenFolder icon.
        //VSHPROPID_OpenFolderIconIndex = -2015,
        public int OpenFolderIconIndex
        {
            get
            {
                return (int)GetProperty(__VSHPROPID.VSHPROPID_OpenFolderIconIndex);
            }
        }
        //
        // Summary:
        //     I4. [optional] Handle of an icon for an open folder. UIHierarchyWindow will not
        //     call DestroyIcon on it.
        //VSHPROPID_OpenFolderIconHandle = -2014,
        public int OpenFolderIconHandle
        {
            get
            {
                return (int)GetProperty(__VSHPROPID.VSHPROPID_OpenFolderIconHandle);
            }
        }
        //
        // Summary:
        //     I4. Handle to an icon; UIHierarchyWindow will not call DestroyIcon on it.
        //VSHPROPID_IconHandle = -2013,
        public int IconHandle
        {
            get
            {
                return (int)GetProperty(__VSHPROPID.VSHPROPID_IconHandle);
            }
        }
        //
        // Summary:
        //     [obsolete] Use Microsoft.VisualStudio.Shell.Interop.__VSHPROPID.VSHPROPID_Name.
        //VSHPROPID_ProjectName = -2012,
        public object ProjectName
        {
            get
            {
                return GetProperty(__VSHPROPID.VSHPROPID_ProjectName);
            }
        }
        //
        // Summary:
        //     BSTR. Name for project (Microsoft.VisualStudio.VSConstants.VSITEMID_ROOT) or
        //     item.
        //VSHPROPID_Name = -2012,
        public string Name
        {
            get
            {
                return GetProperty(__VSHPROPID.VSHPROPID_Name) as string;
            }
        }
        //
        // Summary:
        //     BOOL. Specifies whether the environment should expand this item.
        //VSHPROPID_ExpandByDefault = -2011,
        public bool ExpandByDefault
        {
            get
            {
                return (bool)GetProperty(__VSHPROPID.VSHPROPID_ExpandByDefault);
            }
        }
        //
        // Summary:
        //     BOOL. Specifies whether the environment should display the "open-this-folder"
        //     plus sign.
        //VSHPROPID_Expandable = -2006,
        public int Expandable
        {
            get
            {
                return (int)GetProperty(__VSHPROPID.VSHPROPID_Expandable);
            }
        }
        //
        // Summary:
        //     I4. If Expandable=true, then IconIndex+1 is used for the open icon.
        //VSHPROPID_IconIndex = -2005,
        public int IconIndex
        {
            get
            {
                return (int)GetProperty(__VSHPROPID.VSHPROPID_IconIndex);
            }
        }
        //
        // Summary:
        //     I4. Handle to an imagelist. (Only for itemid==Microsoft.VisualStudio.VSConstants.VSITEMID_ROOT.)
        //VSHPROPID_IconImgList = -2004,
        public int IconImgList
        {
            get
            {
                return (int)GetProperty(__VSHPROPID.VSHPROPID_IconImgList);
            }
        }
        //
        // Summary:
        //     BSTR. Required so the the environment (project window) can display UI.
        //VSHPROPID_Caption = -2003,
        public string Caption
        {
            get
            {
                return GetProperty(__VSHPROPID.VSHPROPID_Caption) as string;
            }
        }
        //
        // Summary:
        //     BSTR. File name specified on the FileSave menu.
        //VSHPROPID_SaveName = -2002,
        public string SaveName
        {
            get
            {
                return GetProperty(__VSHPROPID.VSHPROPID_SaveName) as string;
            }
        }
        //
        // Summary:
        //     GUID to identify the type of node/hierarchy; searches on GUID_ItemType.
        //VSHPROPID_TypeGuid = -1004,
        public Guid TypeGuid
        {
            get
            {
                return (Guid)GetProperty(__VSHPROPID.VSHPROPID_TypeGuid);
            }
        }
        //
        // Summary:
        //     [obsolete] Itemid of Root must be Microsoft.VisualStudio.VSConstants.VSITEMID_ROOT.
        //VSHPROPID_Root = -1003,
        public object Root
        {
            get
            {
                return GetProperty(__VSHPROPID.VSHPROPID_Root);
            }
        }
        //
        // Summary:
        //     INT_PTR. Itemid of next sibling node. (Microsoft.VisualStudio.VSConstants.VSITEMID_NIL
        //     if no more siblings.)
        //VSHPROPID_NextSibling = -1002,
        public VSITEMID NextSibling
        {
            get
            {
                return (VSITEMID)(int)GetProperty(__VSHPROPID.VSHPROPID_NextSibling);
            }
        }
        //
        // Summary:
        //     Itemid of first child node. (Microsoft.VisualStudio.VSConstants.VSITEMID_NIL
        //     if no children.)
        //VSHPROPID_FirstChild = -1001,
        public VSITEMID FirstChild
        {
            get
            {
                return (VSITEMID)(int)GetProperty(__VSHPROPID.VSHPROPID_FirstChild);
            }
        }
        //
        // Summary:
        //     INT_PTR. Specifies the last property ID.
        //VSHPROPID_LAST = -1000,
        public int LAST
        {
            get
            {
                return (int)GetProperty(__VSHPROPID.VSHPROPID_LAST);
            }
        }
        //
        // Summary:
        //     I4. Itemid of the parent node. (Microsoft.VisualStudio.VSConstants.VSITEMID_NIL
        //     if no parent.)
        //VSHPROPID_Parent = -1000,
        public VSITEMID Parent
        {
            get
            {
                return (VSITEMID)(int)GetProperty(__VSHPROPID.VSHPROPID_Parent);
            }
        }
        //
        // Summary:
        //     NIL. -1
        //VSHPROPID_NIL = -1
        public object NIL
        {
            get
            {
                return GetProperty(__VSHPROPID.VSHPROPID_NIL);
            }
        }

        public bool Container
        {
            get
            {
                return (bool)GetProperty(__VSHPROPID2.VSHPROPID_Container);
            }
        }

        //
        // Summary:
        //     BOOL. Indicates whether your project always builds on debug launch (F5) when
        //     the “Only build startup projects and dependencies on Run” Tools option is set.Set
        //     this property to true in your project hierarchy if you want your project to always
        //     build on debug launch in this case. This ensures that your project is built even
        //     if your project is not declared as a build dependency of the startup project.
        //     This property is a workaround for the fact that currently there is only support
        //     for build dependencies (Microsoft.VisualStudio.Shell.Interop.IVsBuildDependency)
        //     and not deploy dependencies in the solution build manager. This property will
        //     be used rarely in circumstances where you may have a circular set of dependencies
        //     involving a combination of build and deploy dependencies between multiple projects
        //     (for example, A has a build dependency on B while B has a deploy dependency on
        //     A; if B is the startup project, then solution build manager would not realize
        //     it needs to build A to satisfy the required deploy dependency when the above
        //     mentioned Tools option is set).
        //VSHPROPID_AlwaysBuildOnDebugLaunch = -2109,
        public bool AlwaysBuildOnDebugLaunch
        {
            get
            {
                return (bool)GetProperty(__VSHPROPID4.VSHPROPID_AlwaysBuildOnDebugLaunch);
            }
        }
        //
        // Summary:
        //     The first ID.
        //VSHPROPID_FIRST4 = -2109,
        public VSITEMID FIRST4
        {
            get
            {
                return (VSITEMID)(int)GetProperty(__VSHPROPID4.VSHPROPID_FIRST4);
            }
        }
        //
        // Summary:
        //     BSTR. A more descriptive name of the item. Used to replace the running document
        //     table moniker in document windows, tooltips, or other parts of the UI.
        //VSHPROPID_DescriptiveName = -2108,
        public string DescriptiveName
        {
            get
            {
                return GetProperty(__VSHPROPID4.VSHPROPID_DescriptiveName) as string;
            }
        }
        //
        // Summary:
        //     BSTR. The build action for an item.
        //VSHPROPID_BuildAction = -2107,
        public string BuildAction
        {
            get
            {
                return GetProperty(__VSHPROPID4.VSHPROPID_BuildAction) as string;
            }
        }
        //
        // Summary:
        //     BSTR. A semicolon-delimited list of the projects required to build this project.
        //     If this property is not implemented, Microsoft.VisualStudio.Shell.Interop.IVsDependencyProvider.EnumDependencies(Microsoft.VisualStudio.Shell.Interop.IVsEnumDependencies@)
        //     will be used.
        //VSHPROPID_BuildDependencies = -2106,
        public string BuildDependencies
        {
            get
            {
                return GetProperty(__VSHPROPID4.VSHPROPID_BuildDependencies) as string;
            }
        }
        //
        // Summary:
        //     BSTR. A semicolon-delimited list of projects required to support design time
        //     features (IntelliSense, form designers, etc.). If this property is not implemented,
        //     Microsoft.VisualStudio.Shell.Interop.__VSHPROPID4.VSHPROPID_BuildDependencies
        //     will be used instead.
        //VSHPROPID_DesignTimeDependencies = -2105,
        public string DesignTimeDependencies
        {
            get
            {
                return GetProperty(__VSHPROPID4.VSHPROPID_DesignTimeDependencies) as string;
            }
        }
        //
        // Summary:
        //     BOOL. Set this property to true in your project hierarchy if your web project
        //     supports being consumed by Silverlight.
        //VSHPROPID_SupportsAspNetIntegration = -2104,
        public bool SupportsAspNetIntegration
        {
            get
            {
                return (bool)GetProperty(__VSHPROPID4.VSHPROPID_SupportsAspNetIntegration);
            }
        }
        //
        // Summary:
        //     BOOL. This item is external to the solution.
        //VSHPROPID_ExternalItem = -2103,
        public bool ExternalItem
        {
            get
            {
                return (bool)GetProperty(__VSHPROPID4.VSHPROPID_ExternalItem);
            }
        }
        //
        // Summary:
        //     VT_BSTR. The format for the target framework moniker is <Identifier>,Version=<ver>,Profile=<value>,
        //     e.g. .NETFramework,Version=v3.5,Profile=Client'.
        //VSHPROPID_TargetFrameworkMoniker = -2102
        public string TargetFrameworkMoniker
        {
            get
            {
                return GetProperty(__VSHPROPID4.VSHPROPID_TargetFrameworkMoniker) as string;
            }
        }



        public override string ToString()
        {
            return this.Caption;
        }
    }
}
