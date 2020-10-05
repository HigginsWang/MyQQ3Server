#pragma once
/// <summary>
/// 0x00000000                  δ��ʼ��
/// 0x00000001-0x000000FF       ��ʾ��˫���͵���Ϣ
/// 0x00000100-0x00000FFF       ��ʾֻ�ܿͻ�������������͵���Ϣ
/// 0x00001000-0x0000FFFF       ��ʾֻ�ܷ�������ͻ��˷��͵���Ϣ
/// 0x00010000-0xFFFFFFFF       δ����
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