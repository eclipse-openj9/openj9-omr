#! /usr/bin/env python

###############################################################################
# Copyright (c) 2018, 2018 IBM Corp. and others
#
# This program and the accompanying materials are made available under
# the terms of the Eclipse Public License 2.0 which accompanies this
# distribution and is available at https://www.eclipse.org/legal/epl-2.0/
# or the Apache License, Version 2.0 which accompanies this distribution and
# is available at https://www.apache.org/licenses/LICENSE-2.0.
#
# This Source Code may also be made available under the following
# Secondary Licenses when the conditions for such availability set
# forth in the Eclipse Public License, v. 2.0 are satisfied: GNU
# General Public License, version 2 with the GNU Classpath
# Exception [1] and GNU General Public License, version 2 with the
# OpenJDK Assembly Exception [2].
#
# [1] https://www.gnu.org/software/classpath/license.html
# [2] http://openjdk.java.net/legal/assembly-exception.html
#
# SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0 WITH Classpath-exception-2.0 OR LicenseRef-GPL-2.0 WITH Assembly-exception
###############################################################################

from jsonschema import Draft6Validator, validate, ValidationError
import os
import sys
import json
import unittest

class ValidateSchemas(unittest.TestCase):

    def test_api_schema(self):
        with open('schema/api.schema.json') as f:
            schema = json.load(f)
            Draft6Validator.check_schema(schema)

    def test_api_type_schema(self):
        with open('schema/api-type.schema.json') as f:
            schema = json.load(f)
            Draft6Validator.check_schema(schema)

    def test_api_field_schema(self):
        with open('schema/api-field.schema.json') as f:
            schema = json.load(f)
            Draft6Validator.check_schema(schema)

    def test_api_service_schema(self):
        with open('schema/api-service.schema.json') as f:
            schema = json.load(f)
            Draft6Validator.check_schema(schema)

    def test_api_class_schema(self):
        with open('schema/api-class.schema.json') as f:
            schema = json.load(f)
            Draft6Validator.check_schema(schema)

    def test_bad_apis(self):
        bad_apis_dir = 'test/bad_api'
        schema = {}
        with open('schema/api.schema.json') as f:
            schema = json.load(f)
        for filename in os.listdir(bad_apis_dir):
            path = os.path.join(bad_apis_dir, filename)
            if sys.version_info >= (3,4):
                # if Python version is >= 3.4, we can use `self.subTest()`
                with self.subTest(filename), open(path) as f, self.assertRaises(ValidationError):
                        validate(json.load(f), schema)
            else:
                with open(path) as f, self.assertRaises(ValidationError):
                        validate(json.load(f), schema)

    def test_minimal_api(self):
        with open('schema/api.schema.json') as schema, open('test/minimal_api.json') as api:
            validate(json.load(api), json.load(schema))

class ValidateJitBuilderAPI(unittest.TestCase):

    @unittest.skipIf(sys.version_info < (3, 0), "requires Python 3 support")
    def test_jitbuilder_api(self):
        schema = {}
        with open('schema/api.schema.json') as f:
            schema = json.load(f)
        with open('jitbuilder.api.json') as f:
            validate(json.load(f), schema)

if __name__ == '__main__':
    unittest.main()
