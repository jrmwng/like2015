//------------------------------------------------------------------------------
// <copyright file="CommandDisasm.cs" company="Company">
//     Copyright (c) Company.  All rights reserved.
// </copyright>
//------------------------------------------------------------------------------

using System;
using System.ComponentModel.Design;
using System.Globalization;
using Microsoft.VisualStudio.Shell;
using Microsoft.VisualStudio.Shell.Interop;

using Microsoft.VisualStudio;
using System.Collections.Generic;

namespace VSIXProject1
{
    /// <summary>
    /// Command handler
    /// </summary>
    internal sealed class CommandDisasm
    {
        /// <summary>
        /// Command ID.
        /// </summary>
        public const int CommandId = 0x0100;

        /// <summary>
        /// Command menu group (command set GUID).
        /// </summary>
        public static readonly Guid CommandSet = new Guid("a9eba2c7-ac20-4032-933a-61baba0047a6");

        /// <summary>
        /// VS Package that provides this command, not null.
        /// </summary>
        private readonly Package package;

        /// <summary>
        /// Initializes a new instance of the <see cref="CommandDisasm"/> class.
        /// Adds our command handlers for menu (commands must exist in the command table file)
        /// </summary>
        /// <param name="package">Owner package, not null.</param>
        private CommandDisasm(Package package)
        {
            if (package == null)
            {
                throw new ArgumentNullException("package");
            }

            this.package = package;

            OleMenuCommandService commandService = this.ServiceProvider.GetService(typeof(IMenuCommandService)) as OleMenuCommandService;
            if (commandService != null)
            {
                var menuCommandID = new CommandID(CommandSet, CommandId);
                var menuItem = new MenuCommand(this.MenuItemCallback, menuCommandID);
                commandService.AddCommand(menuItem);
            }
        }

        /// <summary>
        /// Gets the instance of the command.
        /// </summary>
        public static CommandDisasm Instance
        {
            get;
            private set;
        }

        /// <summary>
        /// Gets the service provider from the owner package.
        /// </summary>
        private IServiceProvider ServiceProvider
        {
            get
            {
                return this.package;
            }
        }

        /// <summary>
        /// Initializes the singleton instance of the command.
        /// </summary>
        /// <param name="package">Owner package, not null.</param>
        public static void Initialize(Package package)
        {
            Instance = new CommandDisasm(package);
        }

        /// <summary>
        /// This function is the callback used to execute the command when the menu item is clicked.
        /// See the constructor to see how the menu item is associated with this function using
        /// OleMenuCommandService service and MenuCommand class.
        /// </summary>
        /// <param name="sender">Event sender.</param>
        /// <param name="e">Event args.</param>
        private void MenuItemCallback(object sender, EventArgs e)
        {
            IVsSolution vsSolution = this.ServiceProvider.GetService(typeof(IVsSolution)) as IVsSolution;
            IVsOutputWindowPane vsOutputPane = null;
            {
                IVsOutputWindow vsOutputWindow = this.ServiceProvider.GetService(typeof(IVsOutputWindow)) as IVsOutputWindow;
                if (!object.ReferenceEquals(vsOutputWindow, null))
                {
                    Guid guidOutputPane = Guid.NewGuid();

                    vsOutputWindow.CreatePane(ref guidOutputPane, @"Disasm", 1, 1);
                    vsOutputWindow.GetPane(ref guidOutputPane, out vsOutputPane);
                }
            }

            if (!object.ReferenceEquals(vsSolution, null))
            {
                if (!object.ReferenceEquals(vsOutputPane, null))
                {
                    string strDir, strFile, strOptFile;
                    vsSolution.GetSolutionInfo(out strDir, out strFile, out strOptFile);

                    vsOutputPane.OutputString(string.Format("{0}({1})\n\t{2}\n\t{3}\n\t{4}\n", @"GetSolutionInfo", vsSolution, strDir, strFile, strOptFile));
                }


                IEnumHierarchies vsProjects;
                vsSolution.GetProjectEnum((uint)__VSENUMPROJFLAGS.EPF_LOADEDINSOLUTION, Guid.Empty, out vsProjects);

                if (!object.ReferenceEquals(vsProjects, null))
                {
                    IVsHierarchy[] aProject = new IVsHierarchy[1];
                    uint uNumOfProject = 0;
                    while (vsProjects.Next((uint)aProject.Length, aProject, out uNumOfProject) == VSConstants.S_OK && uNumOfProject > 0)
                    {
                        IVsProject vsProject = aProject[0] as IVsProject;

                        VsItem vsProjectItem = new VsItem(vsProject);

                        Stack<KeyValuePair<VSConstants.VSITEMID, string>> stackParentPrefix = new Stack<KeyValuePair<VSConstants.VSITEMID, string>>();
                        {
                            stackParentPrefix.Push(new KeyValuePair<VSConstants.VSITEMID, string>(VSConstants.VSITEMID.Root, "\t"));
                        }
                        foreach (VsItem vsItem in vsProjectItem.DescendentsItem)
                        {
                            bool bFound = false;
                            foreach (KeyValuePair<VSConstants.VSITEMID, string> kvParentPrefix in stackParentPrefix)
                            {
                                if (kvParentPrefix.Key.Equals(vsItem.ParentID))
                                {
                                    bFound = true;
                                    break;
                                }
                            }

                            if (!bFound)
                            {
                                stackParentPrefix.Push(new KeyValuePair<VSConstants.VSITEMID, string>(vsItem.ParentID, stackParentPrefix.Peek().Value + "\t"));
                            }
                            while (stackParentPrefix.Peek().Key != vsItem.ParentID)
                            {
                                stackParentPrefix.Pop();
                            }

                            if (!object.ReferenceEquals(vsOutputPane, null))
                            {
                                vsOutputPane.OutputString(string.Format("{0}{1}\n", stackParentPrefix.Peek().Value, vsItem));

                                if (vsItem.Caption.EndsWith(@".cpp"))
                                {
                                    System.Diagnostics.Debugger.Break();
                                }
                            }
                        }
                    }
                }
            }

            IVsMonitorSelection vsSelection = this.ServiceProvider.GetService(typeof(IVsMonitorSelection)) as IVsMonitorSelection;

            if (!object.ReferenceEquals(vsSelection, null))
            {
                // TODO
            }

            string message = string.Format(CultureInfo.CurrentCulture, "Inside {0}.MenuItemCallback()", this.GetType().FullName);
            string title = "CommandDisasm";

            // Show a message box to prove we were here
            VsShellUtilities.ShowMessageBox(
                this.ServiceProvider,
                message,
                title,
                OLEMSGICON.OLEMSGICON_INFO,
                OLEMSGBUTTON.OLEMSGBUTTON_OK,
                OLEMSGDEFBUTTON.OLEMSGDEFBUTTON_FIRST);
        }
    }
}
