function GetTitleStr(id)
	local num = table.getn(titleinfo);
	if id >=1 and id <= num then
		return titleinfo[id]
	else
		return "";
	end
end

function data()
	titleinfo = {
								"[style c=9]大地的精灵[/style]",
								"[style c=1]水的精灵[/style]",
								"[style c=6]火炎精灵[/style]",
								"[style c=4]疾风精灵[/style]",
								"[style c=3]奥萝拉的精灵[/style]",
								"[style c=1]彩[/style][style c=2]虹[/style][style c=5]的[/style][style c=3]精[/style][style c=4]灵[/style]",
								"[style c=7]初心使者[/style]",
								"[style c=7]无名的旅人[/style]",
								"[style c=8]路边的落叶[/style]",
								"[style c=9]水面的小草[/style]",
								"[style c=10]呢喃的歌声[/style]",
								"[style c=11]地上的月影[/style]",
								"[style c=12]奔跑的春风[/style]",
								"[style c=13]苍穹的风云[/style]",
								"[style c=14]摇曳的金星[/style]",
								"[style c=15]欢喜的慈雨[/style]",
								"[style c=16]蕴含的太阳[/style]",
								"[style c=17]敬畏的寂静[/style]",
								"[style c=3]精靈封印者[/style]",
								"[style c=2]五转轮回[/style]",
								"[style c=4]盗贼讨伐者[/style]",
								"[style c=6]盗贼帮众[/style]",
								"[style c=25]夺宝奇兵[/style]",
								"[style c=10]勇闯夺宝岛[/style]",
								"[style c=4]合成达人[/style]",
								"[style c=6]合成高手[/style]",
								"[style c=4]练宠达人[/style]",
								"[style c=6]练宠高手[/style]",
								"[style c=4]任务达人[/style]",
								"[style c=6]任务高手[/style]",
								"[style c=0]初犊战士[/style]",
								"[style c=8]圣灵勇士[/style]",
								"[style c=21]玛丽那丝英雄[/style]",
								"[style c=23]萨姆吉尔英雄[/style]",
								"[style c=18]萨伊那斯使者[/style]",
								"[style c=15]尼斯大陆英雄[/style]"
							}
end

function main()
	data()
end      



                                                  