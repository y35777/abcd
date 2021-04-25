
/*
 *  配置公共部分
 */

//[system]
#define CFG_CO_SEC_SYSTEM                   "system"

#define CFG_CO_BEGIN_TIME                   "begin_time"
#define CFG_CO_END_TIME                     "end_time"
#define CFG_CO_COL_LIST_TIME                "col_list_time"
#define CFG_CO_ONCE_COL_LIST_TIME           "once_col_list_time"   //added by chenliangwei, 2003-08-05
#define CFG_CO_MIN_COL_CYCLE                "min_col_cycle"
#define CFG_CO_COL_SUC_CYCLE                "col_suc_cycle"
#define CFG_CO_CNT_FAIL_CYCLE               "cnt_fail_cycle"
#define CFG_CO_COL_FAIL_CYCLE               "col_fail_cycle"
#define CFG_CO_COL_FAIL_TIMES               "file_col_count"
#define CFG_CO_SWITCH_COUNT                 "switch_count"
#define CFG_CO_ORIG_FILE_SAVE_DAYS          "orig_file_save_days"   //modified by chenliangwei, 2003-07-08
#define CFG_CO_FINAL_FILE_SAVE_DAYS         "final_file_save_days"   //modified by chenliangwei, 2003-07-08
#define CFG_CO_CHECK_SN                     "check_sn" //added by chenliangwei
#define CFG_CO_CHECK_SN_DEPTH               "check_sn_depth" //added by chenliangwei
#define CFG_CREATE_MONTH_FILE               "create_month_file" //added by yangguang 20040218
#define CFG_CREATE_DAY_FILE                 "create_day_file" //added by yangguang 20040218
#define CFG_METER_BILL_DATE                 "meter_bill_date" //added by yangguang 20040218
#define CFG_METER_UPLIMIT                   "meter_uplimit"



#define CFG_CO_BEGIN_TIME_DEFAULT           "00:00"
#define CFG_CO_END_TIME_DEFAULT             "23:59"
#define CFG_CO_COL_LIST_TIME_DEFAULT        ""
#define CFG_CO_ONCE_COL_LIST_TIME_DEFAULT   ""       //added by chenliangwei, 2003-08-05
#define CFG_CO_MIN_COL_CYCLE_DEFAULT        0
#define CFG_CO_COL_SUC_CYCLE_DEFAULT        600
#define CFG_CO_CNT_FAIL_CYCLE_DEFAULT       120
#define CFG_CO_COL_FAIL_CYCLE_DEFAULT       300
#define CFG_CO_COL_FAIL_TIMES_DEFAULT       3
#define CFG_CO_SWITCH_COUNT_DEFAULT         0
#define CFG_CO_ORIG_FILE_SAVE_DAYS_DEFAULT  7   //modified by chenliangwei, 2003-07-08
#define CFG_CO_FINAL_FILE_SAVE_DAYS_DEFAULT 7   //modified by chenliangwei, 2003-07-08
#define CFG_CO_CHECK_SN_DEFAULT             1   //added by chenliangwei
#define CFG_CO_CHECK_SN_DEPTH_DEFAULT       20  //added by chenliangwei
#define CFG_METER_UPLIMIT_DEFAULT           100000000L


//[switch*]
#define CFG_CO_SEC_SWITCH                   "switch"

#define CFG_CO_NAME                         "collect_name"
#define CFG_CO_TYPE                         "collect_type"
#define CFG_CO_PROTOCOL                     "protocol_factory"
#define CFG_CO_LINK_COUNT                   "link_count"
#define CFG_CO_FILE_LEN_VERIFY              "file_len_verify"
#define CFG_CO_FILE_LEVEL_VERIFY            "file_level_verify"
#define CFG_CO_FILE_LEVEL_FAIL_RECOL        "file_level_err_recol"
#define CFG_CO_BILL_LEVEL_VERIFY            "bill_level_verify"
#define CFG_CO_BILL_LEVEL_FAIL_RECOL        "bill_level_err_recol"
#define CFG_CO_CREATE_ZERO_FINAL_FILE       "create_zero_final_file"
#define CFG_CO_ORIG_BILL_LEN                "orig_bill_len"
#define CFG_CO_FORCE_COL_ALL_FILE           "force_col_all_file"


#define CFG_CO_NAME_DEFAULT                 ""
#define CFG_CO_TYPE_DEFAULT                 ""
#define CFG_CO_PROTOCOL_DEFAULT             "Vertel"
#define CFG_CO_LINK_COUNT_DEFAULT           0
#define CFG_CO_FILE_LEN_VERIFY_DEFAULT      0   //added by chenliangwei
#define CFG_CO_FILE_LEVEL_VERIFY_DEFAULT       1
#define CFG_CO_FILE_LEVEL_FAIL_RECOL_DEFAULT   0
#define CFG_CO_BILL_LEVEL_VERIFY_DEFAULT       1
#define CFG_CO_BILL_LEVEL_FAIL_RECOL_DEFAULT   0
#define CFG_CO_CREATE_ZERO_FINAL_FILE_DEFAULT  0 
#define CFG_CO_ORIG_BILL_LEN__DEFAULT          0
#define CFG_CO_FORCE_COL_ALL_FILE_DEFAULT      0 //add by yangguang

//[switch*_子对象*]
#define CFG_CO_MODULE_LABEL                 "module_label"
#define CFG_CO_PRI_LINK                     "pri_link"
#define CFG_CO_BAK_LINK                     "bak_link"
#define CFG_CO_REMOTE_DIR                   "remote_dir"
#define CFG_CO_LOCAL_DIR                    "local_dir"
#define CFG_CO_FINAL_DIR                    "final_dir"
#define CFG_CO_SECOND_FINAL_DIR             "sencond_final_dir"
#define CFG_CO_BILL_TYPE                    "bill_type"

#define CFG_CO_MODULE_LABEL_DEFAULT         ""
#define CFG_CO_PRI_LINK_DEFAULT             ""
#define CFG_CO_BAK_LINK_DEFAULT             ""
#define CFG_CO_REMOTE_DIR_DEFAULT           ""
#define CFG_CO_LOCAL_DIR_DEFAULT            ""
#define CFG_CO_FINAL_DIR_DEFAULT            ""
#define CFG_CO_SECOND_FINAL_DIR_DEFAULT     ""
#define CFG_CO_BILL_TYPE_DEFAULT            ""

//[switch*_link*]
#define CFG_CO_REMOTE_ADDR                  "remote_addr"
#define CFG_CO_REMOTE_USER                  "remote_user"
#define CFG_CO_REMOTE_PWD                   "remote_pwd"
#define CFG_CO_LOCAL_ADDR                   "local_addr"
#define CFG_CO_LOCAL_USER                   "local_user"
#define CFG_CO_LOCAL_PWD                    "local_pwd"
#define CFG_CO_DEV_NAME                     "dev_name"
#define CFG_CO_TRANS_SPEED                  "trans_speed"
#define CFG_CO_LINK_ALARM_PARA              "alarm_para"

#define CFG_CO_REMOTE_ADDR_DEFAULT          ""
#define CFG_CO_REMOTE_USER_DEFAULT          ""
#define CFG_CO_REMOTE_PWD_DEFAULT           ""
#define CFG_CO_LOCAL_ADDR_DEFAULT           ""
#define CFG_CO_LOCAL_USER_DEFAULT           ""
#define CFG_CO_LOCAL_PWD_DEFAULT            ""
#define CFG_CO_DEV_NAME_DEFAULT             ""
#define CFG_CO_TRANS_SPEED_DEFAULT          6
#define CFG_CO_LINK_ALARM_PARA_DEFAULT      -1



/*
 * S1240采集部分
 */

//[switch*]
#define CFG_CO_SWITCH_OFFICE_NUM            "office_num"
#define CFG_CO_SWITCH_CODE                  "switch_code"
#define CFG_CO_TP_COUNT                     "tp_count"
#define CFG_CO_TP_NENAME                    "nename"
#define CFG_CO_ASS_PWD                      "ass_pwd"
#define CFG_CO_ACT_PWD                      "opr_pwd"
#define CFG_CO_DUMPED                       "dumped"
#define CFG_CO_FINAL_STATUS                 "final_status"
#define CFG_CO_FAIL_ALARM_LIMIT             "fail_alarm_limit"

#define CFG_CO_SWITCH_OFFICE_NUM_DEFAULT    ""
#define CFG_CO_SWITCH_CODE_DEFAULT          ""
#define CFG_CO_TP_COUNT_DEFAULT             0
#define CFG_CO_TP_NENAME_DEFAULT            ""
#define CFG_CO_ASS_PWD_DEFAULT              ""
#define CFG_CO_ACT_PWD_DEFAULT              ""
#define CFG_CO_DUMPED_DEFAULT               0
#define CFG_CO_FINAL_STATUS_DEFAULT         "release"
#define CFG_CO_FAIL_ALARM_LIMIT_DEFAULT     10

//[switch*_tp*]
#define CFG_CO_SEC_TP                       "tp"
#define CFG_CO_TP_ID                        "tp_id"

#define CFG_CO_TP_ID_DEFAULT                ""

//[switch*_tp*_cmise_link*]
#define CFG_CO_AGENT_ADDR                   "agent"
#define CFG_CO_MANAGER_ADDR                 "manager"

#define CFG_CO_AGENT_ADDR_DEFAULT           ""
#define CFG_CO_MANAGER_ADDR_DEFAULT         ""



/*
 * 5ESS采集部分
 */
//[switch*]
#define CFG_5ESS_PREFIX_NUM                 "prefix_count"
#define CFG_5ESS_AREA_CODE                  "area_code"

#define CFG_5ESS_PREFIX_NUM_DEFAULT         1
#define CFG_5ESS_AREA_CODE_DEFAULT          "010"

//[switch*_5ess_fna]
#define CFG_5ESS_SAVE_FNA_DIR               "fna_dir"
#define CFG_5ESS_FNA_NAME                   "fna_name"

#define CFG_5ESS_SAVE_FNA_DIR_DEFAULT       "/var/other/fna"
#define CFG_5ESS_FNA_NAME_DEFAULT           "fna.txt"

//[switch*_Prefix*]
#define CFG_CO_SEC_PREFIX                   "Prefix"
#define CFG_5ESS_SEC_PREFIX                 "switch%d_Prefix%d"
#define CFG_5ESS_PREFIX_NAME                "prefix_name"
#define CFG_5ESS_BILL_TYPE                  "bill_type"

#define CFG_5ESS_SEC_PREFIX_DEFAULT         ""
#define CFG_5ESS_PREFIX_NAME_DEFAULT        "stream"
#define CFG_5ESS_BILL_TYPE_DEFAULT          "DDD"


/*
 * EWSD采集部分
 */
//[switch*]
#define CFG_CO_SEC_AMA                      "ama"
#define CFG_CO_COL_AMA_COUNT                "ama_count"

#define CFG_CO_COL_AMA_COUNT_DEFAULT         0

//[switch*_ama*]
#define CFG_EWSD_AMA_FILE                   "ama_file_name"
#define CFG_EWSD_AMA_FILE_SN_LEN            "ama_file_sn_len"

#define CFG_EWSD_AMA_FILE_DEFAULT            ""
#define CFG_EWSD_AMA_FILE_SN_LEN_DEFAULT     4


/*
 * Com采集部分
 */
//[system]
#define CFG_CO_DEL_REMOTE_FILE              "del_remote_file"
#define CFG_CO_COL_LAST_FILE                "col_last_file"//added by chenliangwei,2003-05-16

#define CFG_CO_DEL_REMOTE_FILE_DEFAULT      0   //added by chenliangwei
#define CFG_CO_COL_LAST_FILE_DEFAULT        1   //added by chenliangwei, 2003-05-16

//[switch*]
#define CFG_CO_COL_DIR_COUNT                "col_dir_count" //add by chenliangwei
#define CFG_CO_FILE_SN_LEN                  "file_sn_len" //added by chenliangwei
#define CFG_CO_FILE_POSTFIX_LEN             "file_postfix_len" //added by chenliangwei
#define CFG_CO_FILE_POSTFIX                 "file_postfix" //added by chenliangwei
#define CFG_CO_FILE_PREFIX                  "file_prefix"  //added by chenliangwei
#define CFG_CO_POSTFIX_FILTER_MODE          "postfix_filter_mode" //added by chenliangwei
#define CFG_CO_PREFIX_FILTER_MODE           "prefix_filter_mode" //added by chenliangwei
#define CFG_CO_FILE_SN_UP_LMT               "file_sn_uplimit" //added by chenliangwei
#define CFG_CO_FILE_SN_DOWN_LMT             "file_sn_downlimit" //added by chenliangwei
#define CFG_CO_COL_TRANS_MODE               "trans_mode" //add by chenliangwei


#define CFG_CO_COL_DIR_COUNT_DEFAULT        0    //add by chenliangwei
#define CFG_CO_FILE_SN_LEN_DEFAULT          8   //added by chenliangwei
#define CFG_CO_FILE_POSTFIX_LEN_DEFAULT     3   //added by chenliangwei
#define CFG_CO_FILE_POSTFIX_DEFAULT         ""   //added by chenliangwei
#define CFG_CO_FILE_PREFIX_DEFAULT          ""   //added by chenliangwei
#define CFG_CO_POSTFIX_FILTER_MODE_DEFAULT  1     //added by chenliangwei
#define CFG_CO_PREFIX_FILTER_MODE_DEFAULT   1     //added by chenliangwei
#define CFG_CO_FILE_SN_UP_LMT_DEFAULT       0    //added by chenliangwei
#define CFG_CO_FILE_SN_DOWN_LMT_DEFAULT     0    //added by chenliangwei
#define CFG_CO_COL_TRANS_MODE_DEFAULT       "asc" //add by chenliangwei


//[swith*_col_dir*]
#define CFG_CO_SEC_COL_DIR                  "col_dir"


/*
 *  采集转换对象的配置
 */
#define CFG_CO_METER_FINAL_DIR              "meter_final_dir"
#define CFG_CO_METER_SECOND_FINAL_DIR       "meter_sencond_final_dir"


#define CFG_CO_METER_FINAL_DIR_DEFAULT            ""
#define CFG_CO_METER_SECOND_FINAL_DIR_DEFAULT     ""



