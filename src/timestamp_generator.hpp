/*
  Copyright (c) 2014-2016 DataStax

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
*/

#ifndef __CASS_TIMESTAMP_GENERATOR_HPP_INCLUDED__
#define __CASS_TIMESTAMP_GENERATOR_HPP_INCLUDED__

#include "atomic.hpp"
#include "constants.hpp"
#include "macros.hpp"
#include "ref_counted.hpp"
#include "request.hpp"

#include <stdint.h>

namespace cass {

class TimestampGenerator : public RefCounted<TimestampGenerator> {
public:
  enum Type {
    SERVER_SIDE,
    MONOTONIC
  };

  TimestampGenerator(Type type)
    : type_(type) { }

  virtual ~TimestampGenerator() { }

  Type type() const { return type_; }

  virtual int64_t next() = 0;

private:
  Type type_;

private:
  DISALLOW_COPY_AND_ASSIGN(TimestampGenerator);
};

class ServerSideTimestampGenerator : public TimestampGenerator {
public:
  ServerSideTimestampGenerator()
    : TimestampGenerator(SERVER_SIDE) { }

  virtual int64_t next() { return CASS_INT64_MIN; }
};

class MonotonicTimestampGenerator : public TimestampGenerator {
public:
  MonotonicTimestampGenerator()
    : TimestampGenerator(MONOTONIC)
    , last_(0) { }

  virtual int64_t next();

private:
  int64_t compute_next(int64_t last);

  Atomic<int64_t> last_;
};

} // namespace cass

#endif

