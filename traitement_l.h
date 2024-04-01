#include "redgrammm.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

bool is_unsatified_range(char *str);
bool is_age(char c);
bool is_Cache_Control(char c);
bool is_Expires(char *str);
bool is_Pragma(char c);
bool is_Warning(char c);
bool is_cache_directive(char c);
bool is_delta_seconds(char c);
bool is_extension_pragma(char c);
bool is_pragma_directive(char *str);
bool is_warn_agent(char c);
bool is_warn_code(char *str);
bool is_warn_date(char *str);
bool is_warn_text(char *str);
bool is_warning_value(char *str);
bool is_Proxy_Authenticate(char c);
bool is_Proxy_Authorization(char *str);
bool is_WWW_Authenticate(char c);
bool is_auth_param(char c);
bool is_auth_scheme(char c);
bool is_challenge(char c);
bool is_credentials(char c);
bool is_Authorization(char *str);
bool is_token68(char *str);
bool is_Connection_header(char *str);
bool is_Content_Length_header(char *str);
bool is_Content_Type_header(char *str);
bool is_Trailer_header(char *str);
bool is_Transfer_Encoding_header(char *str);
bool is_Upgrade_header(char *str);
bool is_Via_header(char *str);
bool is_Age_header(char *str);
bool is_Expires_header(char *str);
bool is_Date_header(char *str);
bool is_Location_header(char *str);
bool is_Retry_After_header(char *str);
bool is_Vary_header(char *str);
bool is_Warning_header(char *str);
bool is_Cache_Control_header(char *str);
bool is_Expect_header(char *str);
bool is_Host_header(char *str);
bool is_Max_Forwards_header(char *str);
bool is_Pragma_header(char *str);
bool is_Range_header(char *str);
bool is_TE_header(char *str);
bool is_If_Match_header(char *str);
bool is_If_None_Match_header(char *str);
bool is_If_Modified_Since_header(char *str);
bool is_If_Unmodified_Since_header(char *str);
bool is_If_Range_header(char *str);
bool is_Accept_header(char *str);
bool is_Accept_Charset_header(char *str);
bool is_Accept_Encoding_header(char *str);
bool is_Accept_Language_header(char *str);
bool is_Authorization_header(char *str);
bool is_Proxy_Authorization_header(char *str);
bool is_From_header(char *str);
bool is_Referer_header(char *str);
bool is_User_Agent_header(char *str);
bool is_cookie_name(char *str);
bool is_cookie_value(char *str);
bool is_cookie_octet(char c);
bool is_Cookie_header(char *str);
bool is_Cookie_string(char c);
bool is_header_field(char *str);
bool is_generic_header(char *str);