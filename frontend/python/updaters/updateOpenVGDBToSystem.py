from sqlTableUpdater import SqlTableUpdater
from sqldatabase import SqlDatabase

class OpenVGDBToSystemUpdater(SqlTableUpdater):

    def __init__(self, tableName="", tableColumns=[], coreInfo={}):
        if len(tableColumns) == 0:
            tableColumns = ( 
                             ("openvgdbSystemName", "TEXT"),
                             ("systemUUID", "TEXT" ),
                           )
        SqlTableUpdater.__init__(self, tableName, tableColumns, coreInfo)

    def updateTable(self):
        with SqlDatabase(self.dbFile, autoCommit=True) as db:
            self.updateColumns(db)

            openVGDBToSystemMap = self.getOpenVGDBToPhoenixMap()

            # key, value
            for k, v in openVGDBToSystemMap.iteritems():
                values = [k, v]
                db.insert(self.tableName, self.columnsDict.keys(), values, force=False)
            
if __name__ == "__main__":

    from collections import OrderedDict
    
    updater = SystemToCoreUpdater("systemToCore", columns)
    updater.updateTable()


