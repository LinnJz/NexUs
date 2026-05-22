#ifndef NXWIDGETTOOLS_NXPACKETIO_NXPACKETIO_EXPORT_H_
#define NXWIDGETTOOLS_NXPACKETIO_NXPACKETIO_EXPORT_H_

#ifdef NXPACKETIO_LIBRARY
#if defined(_WIN32)
#if defined(_MSC_VER)
#define NX_PACKETIO_EXPORT __declspec(dllexport)
#else
#define NX_PACKETIO_EXPORT __attribute__((visibility("default")))
#endif
#else
#define NX_PACKETIO_EXPORT __attribute__((visibility("default")))
#endif
#else
#if defined(_WIN32)
#if defined(_MSC_VER)
#define NX_PACKETIO_EXPORT __declspec(dllimport)
#else
#define NX_PACKETIO_EXPORT
#endif
#else
#define NX_PACKETIO_EXPORT __attribute__((visibility("default")))
#endif
#endif

#endif //NXWIDGETTOOLS_NXPACKETIO_NXPACKETIO_EXPORT_H_
