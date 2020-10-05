#pragma once
/// <summary>
/// 0x00000000                  未初始化
/// 0x00000001-0x000000FF       表示可双向发送的消息
/// 0x00000100-0x00000FFF       表示只能客户端向服务器发送的消息
/// 0x00001000-0x0000FFFF       表示只能服务器向客户端发送的消息
/// 0x00010000-0xFFFFFFFF       未定义
/// </summary>
enum class message_type
{
    UNDEFINE = 0x00000000,

    BOTH_TEXT,

    C2S_LOGIN = 0x00000100,
    C2S_GETALLFRIENDS,


    S2C_LOGINRET = 0x00001000,
    S2C_GETALLFRIENDSRET,


    USER_USERID_USERNAME = 0x00010000,
    FILE,
    LINK,
    CHATRECORD



};