/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  Authors : Benjamin GAUTHIER - 24 Mar 2004
 *            Joseph BANINO
 *            Olivier JACQUES
 *            Richard GAYRAUD
 *            From Hewlett Packard Company.
 *            Guillaume Teissier from FTR&D
 */

#include "sipp.hpp"
#include <assert.h>
#ifdef PCAPPLAY
#include "prepare_pcap.h"
#endif


static const char* strIntCmd(CAction::T_IntCmdType type)
{
    switch (type) {
    case CAction::E_INTCMD_STOPCALL:
        return "stop_call";
    case CAction::E_INTCMD_STOP_ALL:
        return "stop_gracefully";
    case CAction::E_INTCMD_STOP_NOW:
        return "stop_now";
    default:
    case CAction::E_INTCMD_INVALID:
        return "invalid";
    }
}

const char* CAction::comparatorToString(T_Comparator comp) const
{
    switch(comp) {
    case E_C_EQ:
        return "==";
    case E_C_NE:
        return "!=";
    case E_C_GT:
        return ">";
    case E_C_LT:
        return "<";
    case E_C_GEQ:
        return ">=";
    case E_C_LEQ:
        return "<=";
    default:
        return "invalid";
    }
}

bool CAction::compare(VariableTable* variableTable) const
{
    double lhs = variableTable->getVar(M_varInId)->getDouble();
    double rhs = M_varIn2Id ? variableTable->getVar(M_varIn2Id)->getDouble() : M_doubleValue;

    switch(M_comp) {
    case E_C_EQ:
        return lhs == rhs;
    case E_C_NE:
        return lhs != rhs;
    case E_C_GT:
        return lhs > rhs;
    case E_C_LT:
        return lhs < rhs;
    case E_C_GEQ:
        return lhs >= rhs;
    case E_C_LEQ:
        return lhs <= rhs;
    default:
        ERROR("Internal error: Invalid comparison type %d", M_comp);
    }
}

void CAction::afficheInfo() const
{
    if (M_action == E_AT_ASSIGN_FROM_REGEXP) {
        if(M_lookingPlace == E_LP_MSG) {
            printf("Type[%d] - regexp[%s] where[%s] - checkIt[%d] - checkItInverse[%d] - $%s",
                   M_action,
                   M_regularExpression.c_str(),
                   "Full Msg",
                   M_checkIt,
                   M_checkItInverse,
                   display_scenario->allocVars->getName(M_varId));
        } else {
            printf("Type[%d] - regexp[%s] where[%s-%s] - checkIt[%d] - checkItInverse[%d] - $%s",
                   M_action,
                   M_regularExpression.c_str(),
                   "Header",
                   M_lookingChar.c_str(),
                   M_checkIt,
                   M_checkItInverse, display_scenario->allocVars->getName(M_varId));
        }
    } else if (M_action == E_AT_EXECUTE_CMD) {
        printf("Type[%d] - command[%-32.32s]", M_action, M_message_str[0].c_str());
    } else if (M_action == E_AT_EXEC_INTCMD) {
        printf("Type[%d] - intcmd[%-32.32s]", M_action, strIntCmd(M_IntCmd));
    } else if (M_action == E_AT_LOG_TO_FILE) {
        printf("Type[%d] - message[%-32.32s]", M_action, M_message_str[0].c_str());
    } else if (M_action == E_AT_LOG_WARNING) {
        printf("Type[%d] - warning[%-32.32s]", M_action, M_message_str[0].c_str());
    } else if (M_action == E_AT_LOG_ERROR) {
        printf("Type[%d] - error[%-32.32s]", M_action, M_message_str[0].c_str());
    } else if (M_action == E_AT_ASSIGN_FROM_SAMPLE) {
        char tmp[40];
        M_distribution->textDescr(tmp, sizeof(tmp));
        printf("Type[%d] - sample varId[%s] %s", M_action, display_scenario->allocVars->getName(M_varId), tmp);
    } else if (M_action == E_AT_ASSIGN_FROM_VALUE) {
        printf("Type[%d] - assign varId[%s] %lf", M_action, display_scenario->allocVars->getName(M_varId), M_doubleValue);
    } else if (M_action == E_AT_ASSIGN_FROM_INDEX) {
        printf("Type[%d] - assign index[%s]", M_action, display_scenario->allocVars->getName(M_varId));
    } else if (M_action == E_AT_ASSIGN_FROM_GETTIMEOFDAY) {
        printf("Type[%d] - assign gettimeofday[%s, %s]", M_action, display_scenario->allocVars->getName(M_varId), display_scenario->allocVars->getName(M_subVarId[0]));
    } else if (M_action == E_AT_ASSIGN_FROM_STRING) {
        printf("Type[%d] - string assign varId[%s] [%-32.32s]", M_action, display_scenario->allocVars->getName(M_varId), M_message_str[0].c_str());
    } else if (M_action == E_AT_JUMP) {
        printf("Type[%d] - jump varInId[%s] %lf", M_action, display_scenario->allocVars->getName(M_varInId), M_doubleValue);
    } else if (M_action == E_AT_PAUSE_RESTORE) {
        printf("Type[%d] - restore pause varInId[%s] %lf", M_action, display_scenario->allocVars->getName(M_varInId), M_doubleValue);
    } else if (M_action == E_AT_VAR_ADD) {
        printf("Type[%d] - add varId[%s] %lf", M_action, display_scenario->allocVars->getName(M_varId), M_doubleValue);
    } else if (M_action == E_AT_VAR_MULTIPLY) {
        printf("Type[%d] - multiply varId[%s] %lf", M_action, display_scenario->allocVars->getName(M_varId), M_doubleValue);
    } else if (M_action == E_AT_VAR_DIVIDE) {
        printf("Type[%d] - divide varId[%s] %lf", M_action, display_scenario->allocVars->getName(M_varId), M_doubleValue);
    } else if (M_action == E_AT_VAR_TRIM) {
        printf("Type[%d] - trim varId[%s]", M_action, display_scenario->allocVars->getName(M_varId));
    } else if (M_action == E_AT_VAR_TEST) {
        printf("Type[%d] - divide varId[%s] varInId[%s] %s %lf", M_action, display_scenario->allocVars->getName(M_varId), display_scenario->allocVars->getName(M_varInId), comparatorToString(M_comp), M_doubleValue);
    } else if (M_action == E_AT_VAR_TO_DOUBLE) {
        printf("Type[%d] - toDouble varId[%s]", M_action, display_scenario->allocVars->getName(M_varId));
#ifdef PCAPPLAY
    } else if ((M_action == E_AT_PLAY_PCAP_AUDIO) || (M_action == E_AT_PLAY_PCAP_IMAGE) || (M_action == E_AT_PLAY_PCAP_VIDEO)) {
        printf("Type[%d] - file[%s]", M_action, M_pcapArgs.file);
#endif

#ifdef RTP_STREAM
  } else if (M_action == E_AT_RTP_STREAM_PLAY) {
      printf("Type[%d] - rtp_stream playfile file %s loop=%d payload %d bytes per packet=%d ms per packet=%d ticks per packet=%d", M_action,M_rtpstream_actinfo.filename,M_rtpstream_actinfo.loop_count,M_rtpstream_actinfo.payload_type,M_rtpstream_actinfo.bytes_per_packet,M_rtpstream_actinfo.ms_per_packet,M_rtpstream_actinfo.ticks_per_packet);
  } else if (M_action == E_AT_RTP_STREAM_PAUSE) {
      printf("Type[%d] - rtp_stream pause", M_action);
  } else if (M_action == E_AT_RTP_STREAM_RESUME) {
      printf("Type[%d] - rtp_stream resume", M_action);
#endif

    } else {
        printf("Type[%d] - unknown action type ... ", M_action);
    }
}

SendingMessage* CAction::getMessage(int n)
{
    return &M_message[n];
}

#ifdef PCAPPLAY
pcap_pkts* CAction::getPcapPkts()
{
    return &M_pcapArgs;
}
#endif

#ifdef RTP_STREAM
rtpstream_actinfo_t* CAction::getRTPStreamActInfo()
{
    return &M_rtpstream_actinfo;
}
#endif

void CAction::setSubVarId(int P_value)
{
    M_subVarId.push_back(P_value);
}

int CAction::getSubVarId(int P_index) const
{
    return M_subVarId[P_index];
}

void CAction::setNbSubVarId(int P_value)
{
    M_subVarId.clear();
    M_subVarId.reserve(P_value);
}

void CAction::setLookingChar(char* P_value)
{
    if (P_value != NULL) {
        M_lookingChar = P_value;
    }
}

void CAction::setMessage(char* P_value, int n)
{
    if (P_value != NULL) {
        /* we can ignore the index (for now) because messages are always
         * pushed in order anyways */
        M_message_str.emplace_back(P_value);
        M_message.emplace_back(M_scenario.get(), P_value, true /* skip sanity */);
    }
}

void CAction::setRegExp(const char* P_value)
{
    int errorCode;

    M_regularExpression = P_value;
    M_regExpSet = true;

    errorCode = regcomp(&M_internalRegExp, P_value, REGEXP_PARAMS);
    if (errorCode != 0) {
        char buffer[MAX_HEADER_LEN];
        regerror(errorCode, &M_internalRegExp, buffer, sizeof(buffer));
        ERROR("recomp error : regular expression '%s' - error '%s'\n", M_regularExpression.c_str(), buffer);
    }
}

const char* CAction::getRegularExpression() const
{
    if (!M_regExpSet) {
        ERROR("Trying to get a regular expression for an action that does not have one!");
    }
    return M_regularExpression.c_str();
}

int CAction::executeRegExp(const char* P_string, VariableTable* P_callVarTable)
{
    regmatch_t pmatch[10];
    int error;
    int nbOfMatch = 0;
    char* result = NULL ;

    if (!M_regExpSet) {
        ERROR("Trying to perform regular expression match on action that does not have one!");
    }

    if (M_subVarId.size() > 9) {
        ERROR("You can only have nine sub expressions!");
    }

    memset((void*)pmatch, 0, sizeof(pmatch));

    error = regexec(&M_internalRegExp, P_string, 10, pmatch, REGEXP_PARAMS);
    if (error == 0) {
        CCallVariable* L_callVar = P_callVarTable->getVar(M_varId);

        for (int i = 0; i <= M_subVarId.size(); i++) {
            if (pmatch[i].rm_eo == -1)
                break;

            setSubString(&result, P_string, pmatch[i].rm_so, pmatch[i].rm_eo);
            L_callVar->setMatchingValue(result);
            nbOfMatch++;

            if (i == M_subVarId.size())
                break;

            L_callVar = P_callVarTable->getVar(getSubVarId(i));
        }
    }
    return nbOfMatch;
}

void CAction::setSubString(char** P_target, const char* P_source, int P_start, int P_stop)
{
    int sizeOf;

    if (P_source != NULL) {
        sizeOf = P_stop - P_start;
        *P_target = new char[sizeOf + 1];

        if (sizeOf > 0) {
            memcpy(*P_target, &P_source[P_start], sizeOf);
        }

        (*P_target)[sizeOf] = '\0';
    } else {
        *P_target = NULL ;
    }
}


#ifdef PCAPPLAY
void CAction::setPcapArgs(char* P_value)
{
    if (P_value != NULL) {
        std::memset(&M_pcapArgs, 0, sizeof(pcap_pkts));
        if (parse_play_args(P_value, &M_pcapArgs) == -1) {
            ERROR("Play pcap error");
        }
    }
}
#endif

#ifdef RTP_STREAM
void CAction::setRTPStreamActInfo(char* P_value)
{
    char* ParamString;
    char* NextComma;

    if (strlen(P_value) >= sizeof(M_rtpstream_actinfo.filename)) {
        ERROR("Filename %s is too long, maximum supported length %zu\n", P_value,
              sizeof(M_rtpstream_actinfo.filename) - 1);
    }
    strcpy(M_rtpstream_actinfo.filename,P_value);
    ParamString = strchr(M_rtpstream_actinfo.filename, ',');
    NextComma = NULL;

    M_rtpstream_actinfo.loop_count = 1;
    if (ParamString) {
        /* we have a loop count parameter */
        *ParamString++ = 0;
        NextComma = strchr(ParamString, ',');
        if (NextComma) {
            *NextComma++ = 0;
        }
        M_rtpstream_actinfo.loop_count = atoi(ParamString);
        ParamString = NextComma;
    }

    M_rtpstream_actinfo.payload_type = rtp_default_payload;
    if (ParamString) {
        /* we have a payload type parameter */
        NextComma = strchr(ParamString,',');
        if (NextComma) {
            *NextComma++ = 0;
        }
        M_rtpstream_actinfo.payload_type = atoi(ParamString);
    }

    /* Setup based on what we know of payload types */
    switch (M_rtpstream_actinfo.payload_type) {
    case 0:
        M_rtpstream_actinfo.ms_per_packet = 20;
        M_rtpstream_actinfo.bytes_per_packet = 160;
        M_rtpstream_actinfo.ticks_per_packet = 160;
        break;
    case 8:
        M_rtpstream_actinfo.ms_per_packet = 20;
        M_rtpstream_actinfo.bytes_per_packet = 160;
        M_rtpstream_actinfo.ticks_per_packet = 160;
        break;
    case 18:
        M_rtpstream_actinfo.ms_per_packet = 20;
        M_rtpstream_actinfo.bytes_per_packet = 20;
        M_rtpstream_actinfo.ticks_per_packet = 160;
        break;
    default:
        M_rtpstream_actinfo.ms_per_packet = -1;
        M_rtpstream_actinfo.bytes_per_packet = -1;
        M_rtpstream_actinfo.ticks_per_packet = -1;
        ERROR("Unknown rtp payload type %d - cannot set playback parameters\n",M_rtpstream_actinfo.payload_type);
        break;
    }

    if (rtpstream_cache_file(M_rtpstream_actinfo.filename) < 0) {
        ERROR("Cannot read/cache rtpstream file %s\n",M_rtpstream_actinfo.filename);
    }
}

void CAction::setRTPStreamActInfo (rtpstream_actinfo_t* P_value)
{
    /* At this stage the entire rtpstream action info structure can simply be */
    /* copied. No members need to be individually duplicated/processed.       */
    memcpy(&M_rtpstream_actinfo, P_value, sizeof(M_rtpstream_actinfo));
}
#endif

#ifdef GTEST
#include "gtest/gtest.h"

TEST(actions, MatchingRegexp) {
    AllocVariableTable vt(NULL);
    int id = vt.find("1", true);
    int sub1_id = vt.find("2", true);
    int sub2_id = vt.find("3", true);
    int sub3_id = vt.find("4", true);
    int sub4_id = vt.find("5", true);
    CAction re(NULL);
    re.M_varId = id;
    re.setNbSubVarId(4);
    re.setSubVarId(sub1_id);
    re.setSubVarId(sub2_id);
    re.setSubVarId(sub3_id);
    re.setSubVarId(sub4_id);
    re.setRegExp("(.+)(o) (.+)(d)");
    int results = re.executeRegExp("hello world", &vt);

    ASSERT_EQ(5, results);
    ASSERT_STREQ("hello world", vt.getVar(id)->getString());
    ASSERT_STREQ("hell", vt.getVar(sub1_id)->getString());
    ASSERT_STREQ("o", vt.getVar(sub2_id)->getString());
    ASSERT_STREQ("worl", vt.getVar(sub3_id)->getString());
    ASSERT_STREQ("d", vt.getVar(sub4_id)->getString());
}

TEST(actions, NonMatchingRegexp) {
    AllocVariableTable vt(NULL);
    int id = vt.find("1", true);
    int sub1_id = vt.find("2", true);
    int sub2_id = vt.find("3", true);
    int sub3_id = vt.find("4", true);
    int sub4_id = vt.find("5", true);
    CAction re(NULL);
    re.M_varId = id;
    re.setNbSubVarId(4);
    re.setSubVarId(sub1_id);
    re.setSubVarId(sub2_id);
    re.setSubVarId(sub3_id);
    re.setSubVarId(sub4_id);
    re.setRegExp("(.+)(o) (.+)(d)");
    int results = re.executeRegExp("", &vt);

    ASSERT_EQ(0, results);
    ASSERT_STREQ("", vt.getVar(id)->getString());
    ASSERT_STREQ("", vt.getVar(sub1_id)->getString());
}

#endif
