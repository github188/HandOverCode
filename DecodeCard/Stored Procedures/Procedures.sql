GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[CardTestN]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[CardTestN]
GO
--CardTestN 解码卡数据升级
CREATE PROCEDURE CardTestN  
WITH   ENCRYPTION 
AS
SET NOCOUNT ON --不返回计数
Set ARITHABORT ON
SELECT [dbo].charDecode(准考证明编号) as 准考证明编号,考车号,[dbo].charDecode(姓名) as MSG0,性别 as MSG1,[dbo].charDecode(流水号) as MSG2,[dbo].charDecode(身份证明编号) as MSG3,驾校名称 as MSG4,考试员1 as MSG5,当日次数 as MSG6,考车号+'-'+SysCfg.备注+'-'+考试车型 as MSG7
FROM StudentInfo 
LEFT JOIN SchoolInfo ON 代理人=驾校编号 
JOIN SysCfg ON 考车号=项目
WHERE 登录状态='已登录' 
AND StudentInfo.状态='3' 
AND (Select CONVERT(varchar(100), [dbo].dateDecode(预考日期), 23)) = (Select CONVERT(varchar(100), GETDATE(), 23))
/*--2014-07-07 ZSZ*/

GO
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[pr_ErrorRecordALL]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[pr_ErrorRecordALL]
GO
--pr_ErrorRecordALL 解码卡数据升级
CREATE PROCEDURE pr_ErrorRecordALL
(
	@ZKZM_Number varchar(50)
) 
WITH   ENCRYPTION 
AS
SET NOCOUNT ON --不返回计数
Set ARITHABORT ON
declare @zkzmbhbase64 varchar(50)
select @zkzmbhbase64=[dbo].charEncode(@ZKZM_Number)
SELECT e.记录编号,e.错误编号,CONVERT(varchar(10),[dbo].dateDecode(e.出错时间),8) as 出错时间,[dbo].charDecode(ed.扣分类型) as 扣分类型,[dbo].intDecode(ed.扣除分数) as 扣除分数 FROM ErrorRecords e,StudentInfo s,ErrorData ed
WHERE e.准考证明编号=s.准考证明编号 
AND e.错误编号=ed.错误编号
AND e.考试次数=s.考试次数 
AND e.当日次数=s.当日次数 
AND e.准考证明编号=@zkzmbhbase64
ORDER BY 记录编号 ASC
/*2014-07-08 ZSZ 参数:准考证明编号 */

GO
--创建pr_ErrorRecords 不存在的时候
if not exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[pr_ErrorRecords]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
begin
exec('CREATE PROCEDURE pr_ErrorRecords
(
	@ZKZM_Number varchar(50),--准考证明编号
	@Ejlbh bigint, --错误记录编号
	@intErrorCode int OUTPUT
) 
WITH   ENCRYPTION 
AS
BEGIN
SET NOCOUNT ON --不返回计数
set @intErrorCode=-1
return @intErrorCode
END
')
end
GO
--修改 pr_ErrorRecords 2014-10-09 加当日次数判断 
ALTER PROCEDURE pr_ErrorRecords
(
	@ZKZM_Number varchar(50),--准考证明编号
	@Ejlbh bigint, --错误记录编号
	@intErrorCode int OUTPUT
) 
WITH   ENCRYPTION 
AS
BEGIN
SET NOCOUNT ON --不返回计数
Set ARITHABORT ON
DECLARE @Status char(1)--状态
declare @KscsN tinyint --考试次数
declare @DrcsN tinyint --当日次数
declare @KSCJ int --成绩
declare @ErrorCode int --错误编号
declare @ErrorCodejl int --记录编号
declare @zkzmbhbase64 varchar(50)
SELECT @zkzmbhbase64=[dbo].charEncode(@ZKZM_Number)
SELECT @Status=状态,@KscsN=考试次数,@DrcsN=当日次数 FROM Studentinfo WHERE 准考证明编号=@zkzmbhbase64
SELECT @KSCJ=100-SUM([dbo].intDecode(扣除分数)) FROM ErrorRecords JOIN ErrorData ON  ErrorRecords.错误编号=ErrorData.错误编号 
WHERE ErrorRecords.准考证明编号=@zkzmbhbase64 and 考试次数=@KscsN and 当日次数=@DrcsN

set @intErrorCode = 0
if exists(Select TOP 1 错误编号 from ErrorRecords where 准考证明编号=@zkzmbhbase64 and 记录编号>@Ejlbh and 当日次数=@DrcsN and 考试次数=@KscsN)
begin --1b
Select TOP 1 @ErrorCode=错误编号,@ErrorCodejl=记录编号 from ErrorRecords where 准考证明编号=@zkzmbhbase64 and 记录编号>@Ejlbh and 当日次数=@DrcsN and 考试次数=@KscsN
if exists(select * from errordata where 扣除分数!=[dbo].intEncode(0, RAND()) and 错误编号=@ErrorCode) 
BEGIN --2b
declare @counterrline int --统计错误行
SELECT @counterrline=Count(*) FROM ErrorRecords er JOIN ErrorData ed ON  er.错误编号=ed.错误编号 
WHERE er.准考证明编号=@zkzmbhbase64 and 考试次数=@KscsN and 当日次数=@DrcsN and ed.扣除分数!=[dbo].intEncode(0, RAND()) and er.记录编号<=@Ejlbh

Select er.记录编号,[dbo].charDecode(ed.扣分类型) as 扣分类型,CONVERT(varchar(10),[dbo].dateDecode(er.出错时间),8) as 出错时间,[dbo].intDecode(ed.扣除分数) as 扣除分数,成绩=@KSCJ,ELine=@counterrline
from ErrorRecords er JOIN ErrorData ed ON er.错误编号=ed.错误编号 
where er.记录编号=@ErrorCodejl

set @intErrorCode = 3
goto ERR_HANDLER
END /*17C53*/ --2e
else
BEGIN --3b
if (@ErrorCode = 201990 OR @ErrorCode=204990 OR @ErrorCode=203990 OR @ErrorCode=206990 OR @ErrorCode=207990 OR @ErrorCode=215990 OR @ErrorCode=216990 )
begin --4b
Select ErrorRecords.记录编号,ErrorRecords.错误编号,[dbo].charDecode(ErrorData.扣分类型) as 扣分类型
from ErrorRecords JOIN ErrorData ON ErrorRecords.错误编号=ErrorData.错误编号 
where ErrorRecords.记录编号=@ErrorCodejl

set @intErrorCode = 5
goto ERR_HANDLER
end --4e
else if((@ErrorCode>201500 and  @ErrorCode<201700) OR (@ErrorCode>203500 and  @ErrorCode<203700) OR (@ErrorCode>204500 and  @ErrorCode<204700)OR (@ErrorCode>206500 and  @ErrorCode<206700) OR (@ErrorCode>207500 and  @ErrorCode<207700)
OR (@ErrorCode>214500 and  @ErrorCode<214700)OR (@ErrorCode>215500 and  @ErrorCode<215700) OR (@ErrorCode>216500 and  @ErrorCode<216700) OR @ErrorCode=10001)
begin --5b
Select er.记录编号,ed.错误编号,[dbo].charDecode(ed.扣分类型) as 扣分类型
from ErrorRecords er JOIN ErrorData ed ON er.错误编号=ed.错误编号 
where er.记录编号=@ErrorCodejl
set @intErrorCode = 2
goto ERR_HANDLER
end --5e
else
begin --6b
set @intErrorCode = 0
goto ERR_HANDLER
end --6e
END /*Not 17C53*/ --3e
end --1e
else
begin --7b
if(@Status =1 OR @Status =2)
BEGIN
	Select 状态=@Status,成绩=@KSCJ,S=127,E=127
	Set @intErrorCode =6
	goto ERR_HANDLER
END
else
BEGIN --8 zt!=1 or zt!=2
	Select 状态=@Status
	Set @intErrorCode=-1
END --8 zt!=1 or zt!=2
end	--7e

ERR_HANDLER:
	return @intErrorCode
END
/*2017-10-02 ZSZ 参数:准考证明编号,误记录编号,返回值*/
GO
