#! /usr/bin/env python

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

if __name__ == '__main__':
    unittest.main()
