/*
 * Copyright (2022) Bytedance Ltd. and/or its affiliates
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <Optimizer/Rule/Rule.h>

namespace DB
{

class PushLimitIntoDistinct: public Rule
{
public:
    RuleType getType() const override { return RuleType::PUSH_LIMIT_INTO_DISTINCT; }
    String getName() const override { return "PUSH_LIMIT_INTO_DISTINCT"; }

    PatternPtr getPattern() const override;

    TransformResult transformImpl(PlanNodePtr node, const Captures & captures, RuleContext & context) override;
};

class PushLimitThroughProjection: public Rule
{
public:
    RuleType getType() const override { return RuleType::PUSH_LIMIT_THROUGH_PROJECTION; }
    String getName() const override { return "PUSH_LIMIT_THROUGH_PROJECTION"; }

    PatternPtr getPattern() const override;

    TransformResult transformImpl(PlanNodePtr node, const Captures & captures, RuleContext & context) override;
};

class PushLimitThroughExtremesStep : public Rule
{
public:
    RuleType getType() const override { return RuleType::PUSH_LIMIT_THROUGH_EXTREMES; }
    String getName() const override { return "PUSH_LIMIT_THROUGH_EXTREMES"; }

    PatternPtr getPattern() const override;

    TransformResult transformImpl(PlanNodePtr node, const Captures & captures, RuleContext & context) override;
};


/**
 * Transforms:
 * <pre>
 * - Limit
 *    - Union
 *       - relation1
 *       - relation2
 *       ..
 * </pre>
 * Into:
 * <pre>
 * - Limit
 *    - Union
 *       - Limit
 *          - relation1
 *       - Limit
 *          - relation2
 *       ..
 * </pre>
 * Applies to LimitNode without ties only to avoid optimizer loop.
 */
class PushLimitThroughUnion: public Rule
{
public:
    RuleType getType() const override { return RuleType::PUSH_LIMIT_THROUGH_UNION; }
    String getName() const override { return "PUSH_LIMIT_THROUGH_UNION"; }

    PatternPtr getPattern() const override;

    TransformResult transformImpl(PlanNodePtr node, const Captures & captures, RuleContext & context) override;
};

/**
 * Transforms:
 * <pre>
 * - Limit
 *    - Join
 *       - left source
 *       - right source
 * </pre>
 * Into:
 * <pre>
 * - Limit
 *    - Join
 *       - Limit (present if Join is left or outer)
 *          - left source
 *       - Limit (present if Join is right or outer)
 *          - right source
 * </pre>
 * Applies to LimitNode without ties only to avoid optimizer loop.
 */
class PushLimitThroughOuterJoin: public Rule
{
public:
    RuleType getType() const override { return RuleType::PUSH_LIMIT_THROUGH_OUTER_JOIN; }
    String getName() const override { return "PUSH_LIMIT_THROUGH_OUTER_JOIN"; }

    PatternPtr getPattern() const override;

    TransformResult transformImpl(PlanNodePtr node, const Captures & captures, RuleContext & context) override;
};


class LimitZeroToReadNothing : public Rule
{
public:
    RuleType getType() const override { return RuleType::LIMIT_ZERO_TO_READNOTHING; }
    String getName() const override { return "LIMIT_ZERO_TO_READNOTHING"; }
    PatternPtr getPattern() const override;

    TransformResult transformImpl(PlanNodePtr node, const Captures & captures, RuleContext & context) override;
};


class PushdownLimitIntoWindow: public Rule
{
public:
    RuleType getType() const override { return RuleType::PUSH_LIMIT_INTO_WINDOW; }
    String getName() const override { return "PUSH_LIMIT_INTO_WINDOW"; }

    PatternPtr getPattern() const override;

    TransformResult transformImpl(PlanNodePtr node, const Captures & captures, RuleContext & context) override;
};

}

