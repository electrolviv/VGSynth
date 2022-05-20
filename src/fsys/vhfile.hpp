#pragma once

#include <stdint.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

class	VHFile {

public:

    enum eMode { read_mode = 0x00, write_mode, append_mode };

    VHFile()                    { _handle = -1; }

    ~VHFile()                   { SafeClose(); }

    bool IsClosed() const       { return !IsOpen(); }

    bool IsOpen() const         { return -1 != _handle; }

    bool SafeOpen( const char *fname, eMode mode ) {

        // Already Open ?
        if( IsOpen() ) return false;

        int param;
        int rights = S_IWUSR | S_IWGRP | S_IWOTH | S_IRUSR | S_IRGRP | S_IROTH;

        if(mode == write_mode) {
            /* unlink(fname); */ param = O_RDWR | O_CREAT;
        } else if(mode == append_mode) {
            if(access( fname, F_OK ) == -1) { param = O_RDWR; }
            else { param = O_WRONLY | O_APPEND; }
        } else if(mode == read_mode) {param = O_RDONLY; }
        else {
            return false;
        }

#ifdef __unix__
        _handle = open(fname, param, rights);
#else
        int access	= O_BINARY | ( ( mode == read_mode ) ? O_RDONLY : O_CREAT|O_WRONLY|O_BINARY ) ;
        int md = ( mode == read_mode ) ? 0 : S_IREAD|S_IWRITE;
        handle = open( name, access, md );
#endif
        _currentMode = mode;
        return IsOpen();
    }

    void SafeClose() {

        // Nothing to close ?
        if(IsClosed()) {
            return;
        }

        close( _handle );
        _handle = -1;
    }

    bool Read(  void *data, uint32_t count ) const {
        if( IsClosed() ) return false;
        if(( !count ) ||( NULL == data ))  return false;
        return read( _handle, data, count ) == count;
    }

    bool Write( const void *data, uint32_t count ) const {
        if( IsClosed() ) return false;
        if(( !count ) ||( NULL == data ))  return false;
        return write( _handle, data, count ) == count;
    }

    bool GetFileSize(const char * cstrfilename, uint32_t *result ) {
        struct stat statbuf;

        if (stat(cstrfilename, &statbuf) == -1) {
            *result = 0;
            return false;
        }

        *result = (uint32_t)statbuf.st_size;

        return true;
    }

    bool Seek(long newpos) {
        lseek(_handle, newpos, SEEK_SET);
        return true;
    }

    bool skeep(uint32_t skipcnt) {
        lseek(_handle, skipcnt, SEEK_CUR);
        return true;
    }

    bool SeekToEnd() {
        lseek(_handle, 0, SEEK_END);
        return true;
    }

    uint32_t getpos() {
        return lseek(_handle, 0, SEEK_CUR);
    }

private:

    int _handle;
    eMode _currentMode;

};

