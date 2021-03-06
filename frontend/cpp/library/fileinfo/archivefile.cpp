#include "archivefile.h"

ArchiveFile::ArchiveFile( const QString &file ) {
    Q_UNUSED( file )
}

const QString ArchiveFile::delimiter() {
    return QStringLiteral( "|||" );
}

const QString ArchiveFile::prefix() {
    return QStringLiteral( "zip://" );
}

ArchiveFile::ParseData ArchiveFile::parse( const QString &file ) {
    QuaZip zip( file );
    zip.open( QuaZip::mdUnzip );

    Q_ASSERT( zip.isOpen() );

    QStringList zipFileList;
    QHash<QString, QString> fileHashMap;

    for( bool f = zip.goToFirstFile(); f; f = zip.goToNextFile() ) {
        // Skip folders (the contents of the folders should still be recursed)
        // Apparently this is guarantied by the standard?
        if( zip.getCurrentFileName().endsWith( '/' ) ) {
            continue;
        }

        const QString absPath = ArchiveFile::prefix() % file
                                % ArchiveFile::delimiter() % zip.getCurrentFileName();
        zipFileList.append( absPath );

        // Grab CRC32 from archive
        QuaZipFileInfo zipFileInfo;

        if( zip.getCurrentFileInfo( &zipFileInfo ) ) {
            fileHashMap.insert( absPath, QString::number( zipFileInfo.crc, 16 ).toUpper() );
        }
    }

    ParseData data;
    data.enumeratedFiles = zipFileList;
    data.fileHashesMap = fileHashMap;

    return data;
}

