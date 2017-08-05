/*++

Copyright (c) 2000  Microsoft Corporation

Module Name:

    macros.h

Abstract:

    Some macros for NDISPROT.

Environment:

    Kernel mode only.

Revision History:

    arvindm     4/5/2000    Created

--*/


#ifndef MIN
#define MIN(_a, _b) ((_a) < (_b)? (_a): (_b))
#endif

#if DBG
#define NPROT_REF_OPEN(_pOpen)   ndisprotDbgRefOpen(_pOpen, __FILENUMBER, __LINE__)
#define NPROT_DEREF_OPEN(_pOpen) ndisprotDbgDerefOpen(_pOpen, __FILENUMBER, __LINE__)
#else
#define NPROT_REF_OPEN(_pOpen)   ndisprotRefOpen(_pOpen)
#define NPROT_DEREF_OPEN(_pOpen) ndisprotDerefOpen(_pOpen)
#endif

//
//  Spinlock macros
//
#if DBG_SPIN_LOCK

#define NPROT_INIT_LOCK(_pLock)              \
           ndisprotAllocateSpinLock(_pLock, __FILENUMBER, __LINE__)
    
#define NPROT_FREE_LOCK(_pLock)              \
           ndisprotFreeSpinLock(_pLock, __FILENUMBER, __LINE__)

#define NPROT_ACQUIRE_LOCK(_pLock)           \
            ndisprotAcquireSpinLock(_pLock, __FILENUMBER, __LINE__)

#define NPROT_RELEASE_LOCK(_pLock)           \
            ndisprotReleaseSpinLock(_pLock, __FILENUMBER, __LINE__)

#define NPROT_FREE_DBG_LOCK()                \
           ndisprotFreeDbgLock()
#else

#define NPROT_INIT_LOCK(_pLock)           NdisAllocateSpinLock(_pLock)
#define NPROT_FREE_LOCK(_pLock)           NdisFreeSpinLock(_pLock)
#define NPROT_ACQUIRE_LOCK(_pLock)        NdisAcquireSpinLock(_pLock)
#define NPROT_RELEASE_LOCK(_pLock)        NdisReleaseSpinLock(_pLock)
#define NPROT_FREE_DBG_LOCK()
    
#endif // DBG

//
//  List manipulation.
//
#define NPROT_INIT_LIST_HEAD(_pList)             InitializeListHead(_pList)
#define NPROT_IS_LIST_EMPTY(_pList)              IsListEmpty(_pList)
#define NPROT_INSERT_HEAD_LIST(_pList, _pEnt)    InsertHeadList(_pList, _pEnt)
#define NPROT_INSERT_TAIL_LIST(_pList, _pEnt)    InsertTailList(_pList, _pEnt)
#define NPROT_REMOVE_ENTRY_LIST(_pEnt)           RemoveEntryList(_pEnt)


//
//  Receive packet queueing.
//
#define NPROT_LIST_ENTRY_TO_RCV_PKT(_pEnt)   \
    CONTAINING_RECORD(CONTAINING_RECORD(_pEnt, NPROT_RECV_PACKET_RSVD, Link), NDIS_PACKET, ProtocolReserved)

#define NPROT_RCV_PKT_TO_LIST_ENTRY(_pPkt)   \
    (&((PNPROT_RECV_PACKET_RSVD)&((_pPkt)->ProtocolReserved[0]))->Link)


//
//  In case we allocate a receive packet of our own to copy and queue
//  received data, we might have to also allocate an auxiliary NDIS_BUFFER
//  to map part of the receive buffer (skipping the header bytes), so as
//  to satisfy NdisTransferData. In such cases, we keep a pointer to the
//  fully mapped receive buffer in the packet reserved space:
//
#define NPROT_RCV_PKT_TO_ORIGINAL_BUFFER(_pPkt)  \
    (((PNPROT_RECV_PACKET_RSVD)&((_pPkt)->ProtocolReserved[0]))->pOriginalBuffer)


//
//  Send packet context.
//
#define NPROT_IRP_FROM_SEND_PKT(_pPkt)        \
    (((PNPROT_SEND_PACKET_RSVD)&((_pPkt)->ProtocolReserved[0]))->pIrp)

#define NPROT_SEND_PKT_RSVD(_pPkt)           \
    ((PNPROT_SEND_PACKET_RSVD)&((_pPkt)->ProtocolReserved[0]))


#define NPROT_REF_SEND_PKT(_pPkt)            \
    (VOID)NdisInterlockedIncrement((PLONG)&NPROT_SEND_PKT_RSVD(_pPkt)->RefCount)


#define NPROT_DEREF_SEND_PKT(_pPkt)          \
    {                                                                               \
        if (NdisInterlockedDecrement((PLONG)&NPROT_SEND_PKT_RSVD(_pPkt)->RefCount) == 0)    \
        {                                                                           \
            NdisFreePacket(_pPkt);                                                  \
        }                                                                           \
    }


#ifdef NDIS51

//
//  Cancel IDs are generated by using the partial cancel ID we got from
//  NDIS ORed with a monotonically increasing locally generated ID.
//
#define NPROT_CANCEL_ID_LOW_MASK     (((ULONG_PTR)-1) >> 8) 

#define NPROT_GET_NEXT_CANCEL_ID()                                                   \
        (PVOID)(Globals.PartialCancelId |                                           \
         ((NdisInterlockedIncrement((PLONG)&Globals.LocalCancelId)) & NPROT_CANCEL_ID_LOW_MASK))

#endif // NDIS51

//
//  Memory allocation
//
#if DBG
#define NPROT_ALLOC_MEM(_pVar, _Size)        \
    (_pVar) = ndisprotAuditAllocMem(         \
                    (PVOID)&(_pVar),        \
                    _Size,                  \
                    __FILENUMBER,           \
                    __LINE__);

#define NPROT_FREE_MEM(_pMem)                \
    ndisprotAuditFreeMem(_pMem);

#else

#define NPROT_ALLOC_MEM(_pVar, _Size)        \
    NdisAllocateMemoryWithTag((PVOID *)(&_pVar), (_Size), NPROT_ALLOC_TAG)

#define NPROT_FREE_MEM(_pMem)                \
    NdisFreeMemory(_pMem, 0, 0)

#endif // DBG


#define NPROT_ZERO_MEM(_pMem, _ByteCount)        \
    NdisZeroMemory(_pMem, _ByteCount)

#define NPROT_COPY_MEM(_pDst, _pSrc, _ByteCount) \
    NdisMoveMemory(_pDst, _pSrc, _ByteCount)

#define NPROT_MEM_CMP(_p1, _p2, _ByteCount)      \
    NdisEqualMemory(_p1, _p2, _ByteCount)

#define NPROT_SET_MEM(_pMem, _ByteVal, _ByteCount)   \
    NdisFillMemory(_pMem, _ByteCount, _ByteVal)

//
//  Events.
//
#define NPROT_INIT_EVENT(_pEvent)            NdisInitializeEvent(_pEvent)
#define NPROT_SIGNAL_EVENT(_pEvent)          NdisSetEvent(_pEvent)
#define NPROT_WAIT_EVENT(_pEvent, _MsToWait) NdisWaitEvent(_pEvent, _MsToWait)


//
//  Flags
//
#define NPROT_SET_FLAGS(_FlagsVar, _Mask, _BitsToSet)    \
        (_FlagsVar) = ((_FlagsVar) & ~(_Mask)) | (_BitsToSet)

#define NPROT_TEST_FLAGS(_FlagsVar, _Mask, _BitsToCheck)    \
        (((_FlagsVar) & (_Mask)) == (_BitsToCheck))


//
//  Block the calling thread for the given duration:
//
#define NPROT_SLEEP(_Seconds)                            \
{                                                       \
    NDIS_EVENT  _SleepEvent;                            \
    NdisInitializeEvent(&_SleepEvent);                  \
    (VOID)NdisWaitEvent(&_SleepEvent, _Seconds*1000);   \
}


#define NDIS_STATUS_TO_NT_STATUS(_NdisStatus, _pNtStatus)                           \
{                                                                                   \
    /*                                                                              \
     *  The following NDIS status codes map directly to NT status codes.            \
     */                                                                             \
    if (((NDIS_STATUS_SUCCESS == (_NdisStatus)) ||                                  \
        (NDIS_STATUS_PENDING == (_NdisStatus)) ||                                   \
        (NDIS_STATUS_BUFFER_OVERFLOW == (_NdisStatus)) ||                           \
        (NDIS_STATUS_FAILURE == (_NdisStatus)) ||                                   \
        (NDIS_STATUS_RESOURCES == (_NdisStatus)) ||                                 \
        (NDIS_STATUS_NOT_SUPPORTED == (_NdisStatus))))                              \
    {                                                                               \
        *(_pNtStatus) = (NTSTATUS)(_NdisStatus);                                    \
    }                                                                               \
    else if (NDIS_STATUS_BUFFER_TOO_SHORT == (_NdisStatus))                         \
    {                                                                               \
        /*                                                                          \
         *  The above NDIS status codes require a little special casing.            \
         */                                                                         \
        *(_pNtStatus) = STATUS_BUFFER_TOO_SMALL;                                    \
    }                                                                               \
    else if (NDIS_STATUS_INVALID_LENGTH == (_NdisStatus))                           \
    {                                                                               \
        *(_pNtStatus) = STATUS_INVALID_BUFFER_SIZE;                                 \
    }                                                                               \
    else if (NDIS_STATUS_INVALID_DATA == (_NdisStatus))                             \
    {                                                                               \
        *(_pNtStatus) = STATUS_INVALID_PARAMETER;                                   \
    }                                                                               \
    else if (NDIS_STATUS_ADAPTER_NOT_FOUND == (_NdisStatus))                        \
    {                                                                               \
        *(_pNtStatus) = STATUS_NO_MORE_ENTRIES;                                     \
    }                                                                               \
    else if (NDIS_STATUS_ADAPTER_NOT_READY == (_NdisStatus))                        \
    {                                                                               \
        *(_pNtStatus) = STATUS_DEVICE_NOT_READY;                                    \
    }                                                                               \
    else                                                                            \
    {                                                                               \
        *(_pNtStatus) = STATUS_UNSUCCESSFUL;                                        \
    }                                                                               \
}


