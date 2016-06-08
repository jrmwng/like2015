using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Microsoft.VisualStudio;
using Microsoft.VisualStudio.Shell.Interop;

namespace VSIXProject1
{
    public class VSITEMID_Enumerable
        : IEnumerable<KeyValuePair<VSConstants.VSITEMID, VSConstants.VSITEMID>>
    {
        private readonly IVsHierarchy m_vsHierarchy;
        private readonly VSConstants.VSITEMID m_vsParentID;

        public VSITEMID_Enumerable(IVsProject vsProject)
        {
            m_vsHierarchy = vsProject as IVsHierarchy;
            m_vsParentID = VSConstants.VSITEMID.Root;
        }
        private VSITEMID_Enumerable(IVsHierarchy vsHierarchy, VSConstants.VSITEMID vsParentID)
        {
            m_vsHierarchy = vsHierarchy;
            m_vsParentID = vsParentID;
        }

        IEnumerator<KeyValuePair<VSConstants.VSITEMID, VSConstants.VSITEMID>> IEnumerable<KeyValuePair<VSConstants.VSITEMID, VSConstants.VSITEMID>>.GetEnumerator()
        {
            object objItemID;
            uint uItemID;

            for (objItemID = null, m_vsHierarchy.GetProperty((uint)m_vsParentID, (int)__VSHPROPID.VSHPROPID_FirstChild, out objItemID); !object.ReferenceEquals(objItemID, null); objItemID = null, m_vsHierarchy.GetProperty(uItemID, (int)__VSHPROPID.VSHPROPID_NextSibling, out objItemID))
            {
                uItemID = (uint)(int)objItemID;

                VSConstants.VSITEMID vsItemID = (VSConstants.VSITEMID)uItemID;

                yield return new KeyValuePair<VSConstants.VSITEMID, VSConstants.VSITEMID>(m_vsParentID, vsItemID);

                foreach (KeyValuePair<VSConstants.VSITEMID, VSConstants.VSITEMID> kvPair in new VSITEMID_Enumerable(m_vsHierarchy, vsItemID))
                {
                    yield return kvPair;
                }
            }
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            object objItemID;
            uint uItemID;

            for (objItemID = null, m_vsHierarchy.GetProperty((uint)m_vsParentID, (int)__VSHPROPID.VSHPROPID_FirstChild, out objItemID); !object.ReferenceEquals(objItemID, null); objItemID = null, m_vsHierarchy.GetProperty(uItemID, (int)__VSHPROPID.VSHPROPID_NextSibling, out objItemID))
            {
                uItemID = (uint)(int)objItemID;

                VSConstants.VSITEMID vsItemID = (VSConstants.VSITEMID)uItemID;

                yield return new KeyValuePair<VSConstants.VSITEMID, VSConstants.VSITEMID>(m_vsParentID, vsItemID);

                foreach (KeyValuePair<VSConstants.VSITEMID, VSConstants.VSITEMID> kvPair in new VSITEMID_Enumerable(m_vsHierarchy, vsItemID))
                {
                    yield return kvPair;
                }
            }
        }
    }
}
