#ifndef COUNTRIESCODE_H
#define COUNTRIESCODE_H

#include <string>
#include <vector>

#define UA_COUNTRY_INDEX 224

// НАКАЗ від 30 грудня 2013 року N 426
// Про затвердження Класифікації країн світу
// Із змінами і доповненнями, внесеними
// наказом Державної служби статистики України
// від 18 грудня 2014 року N 396
// https://zakon.help/article/klasifikaciya-krain-svitu/
struct Country
{
    int code;
    std::string nameAlpha2;
    std::string nameAlpha3;
    std::string nameUA;
    std::string nameENG;
};
class Countries
{
    private: Countries(){}
    private: ~Countries(){}
    public: const std::vector<Country> countriesList =
    {
        { 36,"AU","AUS","Австралія","Australia"},
        { 40,"AT","AUT","Австрія","Austria"},
        { 31,"AZ","AZE","Азербайджан","Azerbaijan"},
        {248,"AX","ALA","Аландські острови","Aland Islands"},
        {  8,"AL","ALB","Албанія","Albania"},
        { 12,"DZ","DZA","Алжир","Algeria"},
        { 16,"AS","ASM","Американське самоа","American Samoa"},
        { 24,"AO","AGO","Ангола","Angola"},
        {660,"AI","AIA","Анґілья","Anguilla"},
        { 20,"AD","AND","Андорра","Andorra"},
        { 10,"AQ","ATA","Антарктика","Antarctica"},
        { 28,"AG","ATG","Антиґуа і барбуда","Antigua and Barbuda"},
        { 32,"AR","ARG","Аргентина","Argentina"},
        {533,"AW","ABW","Аруба","Aruba"},
        {  4,"AF","AFG","Афганістан","Afghanistan"},
        { 44,"BS","BHS","Багамські острови","Bahamas"},
        { 50,"BD","BGD","Бангладеш","Bangladesh"},
        { 52,"BB","BRB","Барбадос","Barbados"},
        { 48,"BH","BHR","Бахрейн","Bahrain"},
        { 84,"BZ","BLZ","Беліз","Belize"},
        { 56,"BE","BEL","Бельґія","Belgium"},
        {204,"BJ","BEN","Бенін","Benin"},
        { 60,"BM","BMU","Бермудські острови","Bermuda"},
        {112,"BY","BLR","Білорусь","Belarus"},
        {100,"BG","BGR","Болгарія","Bulgaria"},
        { 68,"BO","BOL","Болівія","Bolivia, Plurinational State of"},
        {535,"BQ","BES","Бонайре, сінт-естатіус і саба", "Bonaire, Saint Eustatius and Saba"},
        { 70,"BA","BIH","Боснія і герцеґовина","Bosnia and Herzegovina"},
        { 72,"BW","BWA","Ботсвана","Botswana"},
        { 76,"BR","BRA","Бразілія","Brazil"},
        { 86,"IO","IOT","Британська територія в індійському океані","British Indian Ocean Territory"},
        { 96,"BN","BRN","Бруней-даруссалам","Brunei Darussalam"},
        {854,"BF","BFA","Буркіна-фасо","Burkina Faso"},
        {108,"BI","BDI","Бурунді","Burundi"},
        { 64,"BT","BTN","Бутан","Bhutan"},
        {548,"VU","VUT","Вануату","Vanuatu"},
        {826,"GB","GBR","Велика британія","United Kingdom"},
        {862,"VE","VEN","Венесуела","Venezuela (Bolivarian Republic of)"},
        {704,"VN","VNM","В'єтнам","Viet Nam"},
        { 92,"VG","VGB","Вірґінські острови (брит.)","Virgin Islands,  British"},
        {850,"VI","VIR","Вірґінські острови (сша)","Virgin Islands, U.S."},
        { 51,"AM","ARM","Вірменія","Armenia"},
        {266,"GA","GAB","Габон","Gabon"},
        {332,"HT","HTI","Гаїті","Haiti"},
        {270,"GM","GMB","Гамбія","Gambia"},
        {288,"GH","GHA","Гана","Ghana"},
        {324,"GN","GIN","Гвінея","Guinea"},
        {624,"GW","GNB","Гвінея-бісау","Guinea-Bissau"},
        {340,"HN","HND","Гондурас","Honduras"},
        {344,"HK","HKG","Гонконг","Hong Kong"},
        {300,"GR","GRC","Греція","Greece"},
        {268,"GE","GEO","Грузія","Georgia"},
        {328,"GY","GUY","Ґайана","Guyana"},
        {312,"GP","GLP","Ґваделупа","Guadeloupe"},
        {320,"GT","GTM","Ґватемала","Guatemala"},
        {831,"GG","GGY","Ґернсі","Guernsey"},
        {292,"GI","GIB","Ґібралтар","Gibraltar"},
        {308,"GD","GRD","Ґренада","Grenada"},
        {304,"GL","GRL","Ґренландія","Greenland"},
        {316,"GU","GUM","Ґуам","Guam"},
        {208,"DK","DNK","Данія","Denmark"},
        {832,"JE","JEY","Джерсі","Jersey"},
        {262,"DJ","DJI","Джібуті","Djibouti"},
        {212,"DM","DMA","Домініка","Dominica"},
        {214,"DO","DOM","Домініканська республіка","Dominican Republic"},
        {218,"EC","ECU","Еквадор","Ecuador"},
        {226,"GQ","GNQ","Екваторіальна гвінея","Equatorial Guinea"},
        {232,"ER","ERI","Еритрея","Eritrea"},
        {233,"EE","EST","Естонія","Estonia"},
        {231,"ET","ETH","Ефіопія","Ethiopia"},
        {818,"EG","EGY","Єгипет","Egypt"},
        {887,"YE","YEM","Ємен","Yemen"},
        {894,"ZM","ZMB","Замбія","Zambia"},
        {732,"EH","ESH","Західна сахара","Western Sahara"},
        {716,"ZW","ZWE","Зімбабве","Zimbabwe"},
        {400,"JO","JOR","Йорданія","Jordan"},
        {376,"IL","ISR","Ізраїль","Israel"},
        {356,"IN","IND","Індія","India"},
        {360,"ID","IDN","Індонезія","Indonesia"},
        {368,"IQ","IRQ","Ірак","Iraq"},
        {364,"IR","IRN","Іран, ісламська республіка", "Iran, Islamic Republic of"},
        {372,"IE","IRL","Ірландія","Ireland"},
        {352,"IS","ISL","Ісландія","Iceland"},
        {724,"ES","ESP","Іспанія","Spain"},
        {380,"IT","ITA","Італія","Italy"},
        {132,"CV","CPV","Кабо-верде","Cabo Verde"},
        {398,"KZ","KAZ","Казахстан","Kazakhstan"},
        {136,"KY","CYM","Кайманові острови","Cayman Islands"},
        {116,"KH","KHM","Камбоджа","Cambodia"},
        {120,"CM","CMR","Камерун","Cameroon"},
        {124,"CA","CAN","Канада","Canada"},
        {634,"QA","QAT","Катар","Qatar"},
        {404,"KE","KEN","Кенія","Kenya"},
        {417,"KG","KGZ","Киргизстан","Kyrgyzstan"},
        {156,"CN","CHN","Китай","China"},
        {196,"CY","CYP","Кіпр","Cyprus"},
        {296,"KI","KIR","Кірібаті","Kiribati"},
        {166,"CC","CCK","Кокосові (кілінґ) острови","Cocos (Keeling) Islands"},
        {807,"MK","MKD","Колишня югославська республіка македонія","The former Yugoslav Republic of Macedonia"},
        {170,"CO","COL","Колумбія","Colombia"},
        {174,"KM","COM","Комори","Comoros"},
        {178,"CG","COG","Конго","Congo"},
        {180,"CD","COD","Конго, демократична республіка","Congo, Democratic Republic of the"},
        {408,"KP","PRK","Корейська народно-демократична республіка","Korea, Democratic People's Republic"},
        {410,"KR","KOR","Корея, республіка","Korea, Republic of"},
        {188,"CR","CRI","Коста-ріка","Costa Rica"},
        {384,"CI","CIV","Кот-д'івуар","Cote d'Ivoire"},
        {192,"CU","CUB","Куба","Cuba"},
        {414,"KW","KWT","Кувейт","Kuwait"},
        {418,"LA","LAO","Лаоська народно-демократична республіка","Lao People's Democratic Republic"},
        {428,"LV","LVA","Латвія","Latvia"},
        {426,"LS","LSO","Лесото","Lesotho"},
        {440,"LT","LTU","Литва","Lithuania"},
        {430,"LR","LBR","Ліберія","Liberia"},
        {422,"LB","LBN","Ліван","Lebanon"},
        {434,"LY","LBY","Лівія","Libya"},
        {438,"LI","LIE","Ліхтенштейн","Liechtenstein"},
        {442,"LU","LUX","Люксембурґ","Luxembourg"},
        {478,"MR","MRT","Мавританія","Mauritania"},
        {480,"MU","MUS","Маврікій","Mauritius"},
        {450,"MG","MDG","Мадагаскар","Madagascar"},
        {175,"YT","MYT","Майотта","Mayotte"},
        {446,"MO","MAC","Макао","Macao"},
        {454,"MW","MWI","Малаві","Malawi"},
        {458,"MY","MYS","Малайзія","Malaysia"},
        {581,"UM","UMI","Малі віддалені острови сша","United States Minor Outlying Islands"},
        {466,"ML","MLI","Малі","Mali"},
        {462,"MV","MDV","Мальдіви","Maldives"},
        {470,"MT","MLT","Мальта","Malta"},
        {504,"MA","MAR","Марокко","Morocco"},
        {474,"MQ","MTQ","Мартініка","Martinique"},
        {584,"MH","MHL","Маршаллові острови","Marshall Islands"},
        {484,"MX","MEX","Мексика","Mexico"},
        {583,"FM","FSM","Мікронезія, федеративні штати","Micronesia, Federated States of"},
        {508,"MZ","MOZ","Мозамбік","Mozambique"},
        {498,"MD","MDA","Молдова, республіка","Moldova, Republic of"},
        {492,"MC","MCO","Монако","Monaco"},
        {496,"MN","MNG","Монголія","Mongolia"},
        {500,"MS","MSR","Монтсеррат","Montserrat"},
        {104,"MM","MMR","М'янма","Myanmar"},
        {516,"NA","NAM","Намібія","Namibia"},
        {520,"NR","NRU","Науру","Nauru"},
        {524,"NP","NPL","Непал","Nepal"},
        {562,"NE","NER","Нігер","Niger"},
        {566,"NG","NGA","Нігерія","Nigeria"},
        {528,"NL","NLD","Нідерланди","Netherlands"},
        {558,"NI","NIC","Нікараґуа","Nicaragua"},
        {276,"DE","DEU","Німеччина","Germany"},
        {570,"NU","NIU","Ніуе","Niue"},
        {554,"NZ","NZL","Нова зеландія","New Zealand"},
        {540,"NC","NCL","Нова каледонія","New Caledonia"},
        {578,"NO","NOR","Норвеґія","Norway"},
        {784,"AE","ARE","Об'єднані арабські емірати","United Arab Emirates"},
        {512,"OM","OMN","Оман","Oman"},
        { 74,"BV","BVT","Острів буве","Bouvet Island"},
        {334,"HM","HMD","Острів герд і острови макдоналд","Heard Island and McDonald Islands"},
        {833,"IM","IMN","Острів мен","Isle of Man"},
        {574,"NF","NFK","Острів норфолк","Norfolk Island"},
        {162,"CX","CXR","Острів різдва","Christmas Island"},
        {654,"SH","SHN","Острів святої єлени","Saint Helena"},
        {184,"CK","COK","Острови кука","Cook Islands"},
        {796,"TC","TCA","Острови теркс і кайкос","Turks and Caicos Islands"},
        {586,"PK","PAK","Пакистан","Pakistan"},
        {585,"PW","PLW","Палау","Palau"},
        {275,"PS","PSЕ","Палестинська територія","State of Palestine"},
        {591,"PA","PAN","Панама","Panama"},
        {598,"PG","PNG","Папуа-нова ґвінея","Papua New Guinea"},
        {600,"PY","PRY","Параґвай","Paraguay"},
        {604,"PE","PER","Перу","Peru"},
        {710,"ZA","ZAF","Південна африка","South Africa"},
        {239,"GS","SGS","Південна джорджія та південні сандвічеві острови","South Georgia and the South Sandwich Islands"},
        {728,"SS","SSD","Південний судан","South Sudan"},
        {580,"MP","MNP","Північні маріанські острови","Northern Mariana Islands"},
        {612,"PN","PCN","Піткерн","Pitcairn"},
        {616,"PL","POL","Польща","Poland"},
        {620,"PT","PRT","Портуґалія","Portugal"},
        {630,"PR","PRI","Пуерто-ріко","Puerto Rico"},
        {638,"RE","REU","Реюньйон","Runion"},
        {643,"RU","RUS","Російська федерація","Russian Federation"},
        {646,"RW","RWA","Руанда","Rwanda"},
        {642,"RO","ROU","Румунія","Romania"},
        {222,"SV","SLV","Сальвадор","El Salvador"},
        {882,"WS","WSM","Самоа","Samoa"},
        {674,"SM","SMR","Сан-маріно","San Marino"},
        {678,"ST","STP","Сан-томе і прінсіпі","San Tome and Principe"},
        {682,"SA","SAU","Саудівська аравія","Saudi Arabia"},
        {748,"SZ","SWZ","Свазіленд","Swaziland"},
        {336,"VA","VAT","Святий престол (держава-місто ватикан)","Holly See (Vatican City State)"},
        {690,"SC","SYC","Сейшельські острови","Seychelles"},
        {652,"BL","BLM","Сен-бартелемі","Saint Barthlemy"},
        {686,"SN","SEN","Сенеґал","Senegal"},
        {663,"MF","MAF","Сен-мартен (французька частина)","Saint Martin (French part)"},
        {666,"PM","SPM","Сен-п'єр і мікелон","Saint Pierre and Miquelon"},
        {670,"VC","VCT","Сент-вінсент і ґренадіни","Saint Vincent and the Grenadines"},
        {659,"KN","KNA","Сент-кітс і невіс","Saint Kitts and Nevis"},
        {662,"LC","LCA","Сент-люсія","Saint Lucia"},
        {688,"RS","SRB","Сербія","Serbia"},
        {760,"SY","SYR","Сирійська арабська республіка","Syrian Arab Republic"},
        {702,"SG","SGP","Сінгапур","Singapore"},
        {534,"SX","SXM","Сінт-мартен (нідерландська частина)","Sint-Maarten (Dutch part)"},
        {703,"SK","SVK","Словаччина","Slovakia"},
        {705,"SI","SVN","Словенія","Slovenia"},
        { 90,"SB","SLB","Соломонові острови","Solomon Islands"},
        {706,"SO","SOM","Сомалі","Somalia"},
        {729,"SD","SDN","Судан","Sudan"},
        {740,"SR","SUR","Сурінам","Suriname"},
        {840,"US","USA","Сша","United States"},
        {694,"SL","SLE","Сьєрра-леоне","Sierra Leone"},
        {762,"TJ","TJK","Таджикистан","Tajikistan"},
        {158,"TW","TWN","Тайвань, провінція китаю","Taiwan, Province of China"},
        {764,"TH","THA","Таїланд","Thailand"},
        {834,"TZ","TZA","Танзанія, об'єднана республіка","Tanzania, United Republic of"},
        {626,"TL","TLS","Тімор-лешті","Timor-Leste"},
        {768,"TG","TGO","Того","Togo"},
        {772,"TK","TKL","Токелау","Tokelau"},
        {776,"TO","TON","Тонґа","Tonga"},
        {780,"TT","TTO","Трінідад і тобаґо","Trinidad and Tobago"},
        {798,"TV","TUV","Тувалу","Tuvalu"},
        {788,"TN","TUN","Туніс","Tunisia"},
        {792,"TR","TUR","Туреччина","Turkey"},
        {795,"TM","TKM","Туркменістан","Turkmenistan"},
        {800,"UG","UGA","Уганда","Uganda"},
        {348,"HU","HUN","Угорщина","Hungary"},
        {860,"UZ","UZB","Узбекистан","Uzbekistan"},
        {804,"UA","UKR","Україна","Ukraine"},
        {876,"WF","WLF","Уолліс і футуна","Wallis and Futuna Islands"},
        {858,"UY","URY","Уруґвай","Uruguay"},
        {234,"FO","FRO","Фарерські острови","Faroe Islands"},
        {242,"FJ","FJI","Фіджі","Fiji"},
        {608,"PH","PHL","Філіппіни","Philippines"},
        {246,"FI","FIN","Фінляндія","Finland"},
        {238,"FK","FLK","Фолклендські (мальвінські) острови","Falkland Islands (Malvinas)"},
        {250,"FR","FRA","Франція","France"},
        {254,"GF","GUF","Французька ґвіана","French Guiana"},
        {258,"PF","PYF","Французька полінезія","French Polynesia"},
        {260,"TF","ATF","Французькі південні території","French Southern Territories"},
        {191,"HR","HRV","Хорватія","Croatia"},
        {140,"CF","CAF","Центральноафриканська республіка","Central African Republic"},
        {148,"TD","TCD","Чад","Chad"},
        {203,"CZ","CZE","Чехія","Czech Republic"},
        {152,"CL","CHL","Чілі","Chile"},
        {499,"ME","MNE","Чорногорія","Montenegro"},
        {756,"CH","CHE","Швейцарія","Switzerland"},
        {752,"SE","SWE","Швеція","Sweden"},
        {744,"SJ","SJM","Шпіцберґен та ян-маєн","Svalbard and Jan Mayen"},
        {144,"LK","LKA","Шрі-ланка","Sri Lanka"},
        {388,"JM","JAM","Ямайка","Jamaica"},
        {392,"JP","JPN","Японія","Japan"}
    };
    public: const Country &getCountryByCode(const int code) const noexcept
    {
        for(std::vector<Country>::const_iterator c = countriesList.cbegin(); c!=countriesList.cend(); c++)
            if(c->code == code) return *c;
    }
    public: const Country &getCountryByNameAlpha2(std::string nameAlpha2) const noexcept
    {
        for(std::vector<Country>::const_iterator c = countriesList.cbegin(); c!=countriesList.cend(); c++)
            if(nameAlpha2.compare(c->nameAlpha2)==0) return *c;
    }
    public: const Country &getCountryByNameAlpha3(std::string nameAlpha3) const noexcept
    {
        for(std::vector<Country>::const_iterator c = countriesList.cbegin(); c!=countriesList.cend(); c++)
            if(nameAlpha3.compare(c->nameAlpha3)==0) return *c;
    }
    public: const Country &getCountryByNameUA(std::string nameUA) const noexcept
    {
        for(std::vector<Country>::const_iterator c = countriesList.cbegin(); c!=countriesList.cend(); c++)
            if(nameUA.compare(c->nameUA)==0) return *c;
    }
    public: const Country &getCountryByNameENG(std::string nameENG) const noexcept
    {
        for(std::vector<Country>::const_iterator c = countriesList.cbegin(); c!=countriesList.cend(); c++)
            if(nameENG.compare(c->nameENG)==0) return *c;
    }
    public : static Countries& instance(){
        static Countries _instanceOfCountries;
        return _instanceOfCountries;}
};

#endif // COUNTRIESCODE_H

