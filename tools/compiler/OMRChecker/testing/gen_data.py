###############################################################################
#
# (c) Copyright IBM Corp. 2016, 2016
#
#  This program and the accompanying materials are made available
#  under the terms of the Eclipse Public License v1.0 and
#  Apache License v2.0 which accompanies this distribution.
#
#      The Eclipse Public License is available at
#      http://www.eclipse.org/legal/epl-v10.html
#
#      The Apache License v2.0 is available at
#      http://www.opensource.org/licenses/apache2.0.php
#
# Contributors:
#    Multiple authors (IBM Corp.) - initial implementation and documentation
###############################################################################

import os
import sys
import json

def genFileList(dataDir, pred=None):
   if pred is None:
      return [os.path.join(dirpath, fileName) for dirpath, dirs, files in os.walk(dataDir) for fileName in files]
   else:
      return [os.path.join(dirpath, fileName) for dirpath, dirs, files in os.walk(dataDir) for fileName in files if pred(fileName)]

if __name__ == "__main__":

   fileList = genFileList(os.path.join(os.getcwd(), sys.argv[1]))

   f = open(sys.argv[1].replace('/','_') + ".json", "w")
   f.write(json.dumps(fileList, indent=3, separators=(',', ': ')))
   f.close()
