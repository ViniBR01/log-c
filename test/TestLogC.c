#include <string.h>
#include "unity.h"
#include "log_c.h"

static char test_buffer[512];
static size_t test_length;

static void test_output_callback(const char* message, size_t length) {
    if (length < sizeof(test_buffer)) {
        memcpy(test_buffer, message, length);
        test_length = length;
        test_buffer[length] = '\0';
    }
}

void setUp(void) {
    test_length = 0;
    test_buffer[0] = '\0';
    log_set_output_callback(test_output_callback);
}

void tearDown(void) {
    log_set_output_callback(NULL);
}

void test_LogLevel_Info(void) {
    loginfo("Test message");
    
    TEST_ASSERT_TRUE(strstr(test_buffer, "[info]") != NULL);
    TEST_ASSERT_TRUE(strstr(test_buffer, "Test message") != NULL);
}

void test_LogLevel_Error(void) {
    logerror("Error message");
    
    TEST_ASSERT_TRUE(strstr(test_buffer, "[error]") != NULL);
    TEST_ASSERT_TRUE(strstr(test_buffer, "Error message") != NULL);
}

void test_LogLevel_Warning(void) {
    logwarning("Warning message");
    
    TEST_ASSERT_TRUE(strstr(test_buffer, "[warning]") != NULL);
    TEST_ASSERT_TRUE(strstr(test_buffer, "Warning message") != NULL);
}

void test_LogLevel_Debug(void) {
    logdebug("Debug message");
    
    TEST_ASSERT_TRUE(strstr(test_buffer, "[debug]") != NULL);
    TEST_ASSERT_TRUE(strstr(test_buffer, "Debug message") != NULL);
}

void test_LogLevel_Critical(void) {
    logcritical("Critical message");
    
    TEST_ASSERT_TRUE(strstr(test_buffer, "[critical]") != NULL);
    TEST_ASSERT_TRUE(strstr(test_buffer, "Critical message") != NULL);
}

void test_Formatting_Integer(void) {
    loginfo("Number: %d", -42);
    
    TEST_ASSERT_TRUE(strstr(test_buffer, "Number: -42") != NULL);
}

void test_Formatting_UnsignedInteger(void) {
    loginfo("Unsigned: %u", 12345);
    
    TEST_ASSERT_TRUE(strstr(test_buffer, "Unsigned: 12345") != NULL);
}

void test_Formatting_Hexadecimal(void) {
    loginfo("Hex: %x", 0xDEADBEEF);
    
    TEST_ASSERT_TRUE(strstr(test_buffer, "Hex: deadbeef") != NULL);
}

void test_Formatting_String(void) {
    loginfo("String: %s", "hello");
    
    TEST_ASSERT_TRUE(strstr(test_buffer, "String: hello") != NULL);
}

void test_Formatting_Character(void) {
    loginfo("Char: %c", 'X');
    
    TEST_ASSERT_TRUE(strstr(test_buffer, "Char: X") != NULL);
}

void test_Formatting_Multiple(void) {
    loginfo("Mix: %d %s %x", 42, "test", 0xFF);
    
    TEST_ASSERT_TRUE(strstr(test_buffer, "Mix: 42 test ff") != NULL);
}

void test_Formatting_PercentEscape(void) {
    loginfo("Percent: %%");
    
    TEST_ASSERT_TRUE(strstr(test_buffer, "Percent: %") != NULL);
}

void test_EndsWithNewline(void) {
    loginfo("Test");
    
    TEST_ASSERT_GREATER_THAN(0, test_length);
    TEST_ASSERT_EQUAL_CHAR('\n', test_buffer[test_length - 1]);
}

void test_RuntimeFiltering_DefaultLevel(void) {
    // Default level should be compile-time level
    TEST_ASSERT_EQUAL(LOG_LEVEL, log_get_level());
    TEST_ASSERT_EQUAL(LOG_LEVEL, log_get_compile_time_level());
}

void test_RuntimeFiltering_CanSuppress(void) {
    log_set_level(LOG_LEVEL_INFO);
    
    // Info should print
    test_length = 0;
    loginfo("Should print");
    TEST_ASSERT_GREATER_THAN(0, test_length);
    
    // Debug should be suppressed (if compiled in)
    test_length = 0;
    logdebug("Should not print");
    #if LOG_LEVEL >= LOG_LEVEL_DEBUG
    TEST_ASSERT_EQUAL(0, test_length);  // Compiled in but suppressed
    #else
    // Not compiled in, so test_length stays 0 anyway
    TEST_ASSERT_EQUAL(0, test_length);
    #endif
}

void test_RuntimeFiltering_CanReenable(void) {
    log_set_level(LOG_LEVEL_INFO);
    
    test_length = 0;
    logdebug("Suppressed");
    #if LOG_LEVEL >= LOG_LEVEL_DEBUG
    TEST_ASSERT_EQUAL(0, test_length);  // Suppressed
    #endif
    
    log_set_level(LOG_LEVEL_DEBUG);
    test_length = 0;
    logdebug("Now prints");
    #if LOG_LEVEL >= LOG_LEVEL_DEBUG
    TEST_ASSERT_GREATER_THAN(0, test_length);  // Now prints
    #endif
}

void test_RuntimeFiltering_ClampsToMax(void) {
    // Try to set above compile-time max (should clamp)
    log_set_level(LOG_LEVEL_DEBUG + 1);
    TEST_ASSERT_LESS_OR_EQUAL(LOG_LEVEL, log_get_level());
    
    // Should clamp to compile-time max
    TEST_ASSERT_EQUAL(LOG_LEVEL, log_get_level());
}

void test_RuntimeFiltering_ErrorAlwaysPrints(void) {
    // Even if we set to OFF, errors should print if compiled in
    log_set_level(LOG_LEVEL_ERROR);
    
    test_length = 0;
    logerror("Error message");
    TEST_ASSERT_GREATER_THAN(0, test_length);
    
    // But info should be suppressed
    test_length = 0;
    loginfo("Info suppressed");
    TEST_ASSERT_EQUAL(0, test_length);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_LogLevel_Info);
    RUN_TEST(test_LogLevel_Error);
    RUN_TEST(test_LogLevel_Warning);
    RUN_TEST(test_LogLevel_Debug);
    RUN_TEST(test_LogLevel_Critical);
    RUN_TEST(test_Formatting_Integer);
    RUN_TEST(test_Formatting_UnsignedInteger);
    RUN_TEST(test_Formatting_Hexadecimal);
    RUN_TEST(test_Formatting_String);
    RUN_TEST(test_Formatting_Character);
    RUN_TEST(test_Formatting_Multiple);
    RUN_TEST(test_Formatting_PercentEscape);
    RUN_TEST(test_EndsWithNewline);
    RUN_TEST(test_RuntimeFiltering_DefaultLevel);
    RUN_TEST(test_RuntimeFiltering_CanSuppress);
    RUN_TEST(test_RuntimeFiltering_CanReenable);
    RUN_TEST(test_RuntimeFiltering_ClampsToMax);
    RUN_TEST(test_RuntimeFiltering_ErrorAlwaysPrints);
    return UNITY_END();
}
