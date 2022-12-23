
#include "data.h"

float w1[20][25] = {
{	0.427151868	,	0.278845531	,	-1.223999424	,	0.411002202	,	0.434357186	,	0.074335738	,	-0.428521094	,	-0.300304346	,	1.088867008	,	-0.513019593	,	-0.552711647	,	-0.111985723	,	-0.21444721	,	0.166062371	,	-1.16525553	,	0.631599508	,	0.414636333	,	0.845222976	,	0.387488498	,	-0.636918452	,	-0.505925833	,	-0.157621468	,	0.364308481	,	-1.81413628	,	1.259081725	},
{	-0.452000618	,	0.421409942	,	0.135443608	,	0.223449777	,	-0.981347519	,	-0.560635975	,	-0.94786997	,	-0.388385494	,	0.765542252	,	0.494431367	,	0.451318662	,	0.996525182	,	-0.254041749	,	0.542438816	,	-0.461377584	,	-0.645516443	,	-0.682623514	,	0.196202375	,	0.214634613	,	-0.102990257	,	0.799016463	,	-1.151634881	,	-0.399550904	,	0.188861259	,	0.348206139	},
{	0.103631145	,	-0.594961903	,	-0.277656925	,	-0.548713906	,	0.93248194	,	-1.018440204	,	-0.871337436	,	0.612854863	,	0.695567112	,	-0.211616685	,	-0.612348173	,	0.444680471	,	-0.102053757	,	0.484859412	,	0.151780236	,	0.294976527	,	-0.559933974	,	-0.388395856	,	0.192465062	,	-0.187201713	,	-0.067456366	,	1.044038515	,	-0.041790968	,	-0.335761102	,	0.878379923	},
{	-0.804632727	,	0.564567787	,	-0.829382481	,	0.471675727	,	-0.428927512	,	-0.53003208	,	-0.897560617	,	-0.888224287	,	0.742942211	,	0.788393492	,	-0.111265924	,	0.915092011	,	-0.626841619	,	0.717315412	,	0.039536728	,	-0.498019587	,	-0.477016535	,	-0.833078947	,	0.789848018	,	-0.245319436	,	-0.33646621	,	-0.132040203	,	0.271238268	,	0.331045232	,	-0.994125905	},
{	0.01776194	,	-0.372283647	,	-0.282896626	,	-0.486961425	,	-0.314555795	,	0.366294917	,	0.885218693	,	0.192898744	,	0.551209214	,	0.35593196	,	0.597479905	,	0.937008455	,	-0.743149316	,	-0.398976716	,	0.245237886	,	-0.842770114	,	-0.906337697	,	-0.430700903	,	-0.009210833	,	-0.327442279	,	0.625586599	,	0.069678203	,	0.991732993	,	-0.372273105	,	-0.913908208	},
{	0.797688122	,	-0.438292019	,	-1.39088181	,	-0.069208091	,	1.077346021	,	-0.120341245	,	-0.309614456	,	-0.906026245	,	-1.114931095	,	-0.785780855	,	0.447016773	,	-0.33209552	,	-1.09027151	,	-1.401234792	,	-0.378656814	,	0.426745172	,	0.636388366	,	-1.071894725	,	-0.946276925	,	0.196811171	,	0.032427829	,	1.069789561	,	-0.785868483	,	0.836842483	,	0.263693717	},
{	-1.138652923	,	0.486219131	,	-0.555655054	,	0.328506287	,	-0.957643021	,	-0.583945065	,	0.841431149	,	0.68879727	,	0.319442082	,	-0.138970341	,	0.839648356	,	0.672096565	,	-0.383801245	,	1.180060227	,	-0.182755747	,	-1.07995447	,	-0.053742873	,	-0.163076097	,	-0.803089222	,	-0.332324065	,	-0.751628409	,	-0.713464864	,	-0.047707037	,	0.118323722	,	-1.009167419	},
{	0.553959904	,	0.027347884	,	1.049805992	,	-1.378427187	,	-0.563176973	,	-0.838310498	,	-0.748162971	,	0.067551832	,	-1.105508111	,	0.239822168	,	0.713360681	,	-0.705459953	,	0.289649972	,	0.298557871	,	0.592729969	,	0.761859023	,	0.026610502	,	-1.091136981	,	-0.212979617	,	-1.286099998	,	0.221578549	,	0.537703911	,	-0.59087018	,	0.037126042	,	-1.339400055	},
{	0.226399581	,	-0.089069572	,	-1.151138661	,	-0.37052225	,	0.88139658	,	0.44370983	,	0.738108244	,	0.352570302	,	0.983293734	,	1.152513557	,	1.009014819	,	1.084825682	,	0.952168754	,	0.445175012	,	-0.565975585	,	0.885213653	,	0.935861149	,	0.555316276	,	1.177382937	,	0.313104893	,	0.211303669	,	-0.368273406	,	-0.937816945	,	0.613339894	,	0.870315082	},
{	-0.49043734	,	0.926868912	,	0.949861297	,	-1.18047856	,	0.059827924	,	0.881079788	,	0.571823268	,	-0.066363759	,	-0.290980945	,	-0.178321697	,	-0.82263871	,	0.492700541	,	-0.435017441	,	0.846746401	,	-0.089691028	,	0.099215914	,	-0.157598797	,	0.009710764	,	0.170228628	,	0.327218099	,	0.060614	,	-1.614355239	,	0.610622181	,	-0.613092654	,	-0.92278761	},
{	0.178027921	,	-1.035501937	,	-0.202461255	,	0.574361865	,	0.759791059	,	-1.332260963	,	0.343244427	,	0.959412636	,	-0.022311782	,	-0.384358385	,	0.049831113	,	0.354204582	,	-1.080994939	,	0.184646479	,	-0.83085214	,	0.396433392	,	0.309535353	,	0.600647976	,	-0.56207515	,	-1.081026792	,	-0.711401299	,	0.737955788	,	0.058208166	,	-0.702386764	,	1.170374525	},
{	-0.824112147	,	0.079231196	,	0.171168522	,	-0.139469351	,	0.23382454	,	-0.721957662	,	0.206784126	,	0.340847225	,	-0.157736005	,	-0.464883879	,	0.534638188	,	0.108997069	,	0.022719422	,	1.589268166	,	-0.08189464	,	-0.88245068	,	0.021316116	,	-0.228428855	,	-0.569133044	,	-0.256461853	,	-0.367698185	,	0.054521806	,	0.254041354	,	0.312441554	,	-0.556412772	},
{	0.182476182	,	-0.006452143	,	0.937019612	,	-1.056923037	,	0.42831527	,	-0.135688592	,	0.363013439	,	0.605977775	,	-1.13251388	,	0.078824277	,	-0.384996941	,	0.061307371	,	0.868035507	,	-0.799341576	,	0.52479443	,	-1.142126792	,	-0.098245075	,	-0.53384444	,	0.639126038	,	-0.705935239	,	0.462210005	,	-0.287254122	,	-0.051827725	,	0.428585458	,	-0.211084479	},
{	0.151170959	,	-0.800131913	,	-0.918807842	,	0.317511539	,	-0.623986779	,	-0.575632058	,	0.928234453	,	-0.55623928	,	-0.017339694	,	-0.218234749	,	-0.096258833	,	0.17076533	,	-0.868265188	,	-0.281488194	,	-0.12826225	,	0.804306853	,	1.030409273	,	-0.307768078	,	0.873172957	,	-0.798011895	,	0.455511047	,	-0.576917719	,	0.784034738	,	-0.081185601	,	-0.172736356	},
{	0.470906629	,	-0.02351512	,	0.817379518	,	-0.051727866	,	-0.963496186	,	0.109363687	,	-0.598978916	,	-0.576489242	,	-0.717107602	,	-0.65153521	,	0.109550488	,	-1.044189431	,	-0.813154613	,	-1.397917614	,	0.672667078	,	-0.809754697	,	-1.178062347	,	0.506733187	,	0.297632289	,	0.412920053	,	0.544236187	,	0.111513608	,	0.375175403	,	0.252666094	,	-0.513118692	},
{	-0.797423956	,	-0.583436461	,	-1.599173704	,	-0.501373508	,	-0.689625772	,	-0.099666964	,	0.475491064	,	-0.785946676	,	-0.317287632	,	0.75678997	,	0.702765249	,	-0.242167134	,	0.192532442	,	-0.753959679	,	0.010008784	,	-0.169360122	,	-0.036940089	,	-0.657086611	,	-1.184933421	,	0.462597523	,	-1.210523572	,	0.945850285	,	0.185147235	,	0.40735143	,	-1.097230729	},
{	-0.370719532	,	-0.028241787	,	-0.880880042	,	0.137803542	,	1.305232034	,	0.799504227	,	0.440253332	,	0.940973514	,	-0.213765699	,	0.90544752	,	-0.4415546	,	0.983710213	,	0.371835277	,	-0.375300979	,	0.027361463	,	-0.221079048	,	0.947261393	,	1.349589658	,	0.330854648	,	0.585769498	,	-0.412077372	,	0.143046653	,	-0.742312964	,	0.83518044	,	0.186384379	},
{	-0.423563868	,	-0.505035855	,	-0.353429024	,	-0.358954183	,	0.695549165	,	-1.184277149	,	0.983299105	,	-0.460530775	,	0.527868981	,	-0.866830951	,	-1.010701627	,	0.69239036	,	0.774285071	,	0.54266481	,	0.200172084	,	0.137240432	,	0.198815961	,	-0.085798184	,	0.148000774	,	-0.637742936	,	-0.039860861	,	0.137531061	,	-0.605147147	,	0.59570526	,	-0.316659227	},
{	0.205050505	,	0.668932179	,	-0.292302672	,	-1.809616177	,	0.190683901	,	-0.162181698	,	-0.28258902	,	0.578743419	,	-0.516574547	,	0.478439799	,	-0.015160927	,	-0.154988119	,	-0.069633003	,	-1.121684565	,	-0.382149317	,	0.234455499	,	0.600385292	,	-0.221146977	,	-0.877654422	,	-1.332681468	,	0.620935749	,	0.165373017	,	-0.601881307	,	0.965315324	,	-1.175734811	},
{	0.313730642	,	-0.210456434	,	-1.089987299	,	0.577914327	,	1.309326934	,	-0.123704287	,	-0.922862498	,	-1.010196826	,	0.82469441	,	0.15844577	,	0.003005877	,	-0.640176862	,	-1.142343621	,	-1.005382534	,	0.710036033	,	-0.650422421	,	0.05600959	,	-0.847548377	,	0.263090634	,	-0.235740043	,	0.319860725	,	1.436007089	,	-0.40308408	,	0.094978672	,	0.601866711	}
};

float w2[20][20] = {
{	0.037646132	,	-1.140576277	,	0.3594272	,	0.406883597	,	-0.942061518	,	1.107615841	,	-0.396994574	,	-1.09585505	,	0.227185167	,	-1.34972066	,	1.483178037	,	-1.009209836	,	-0.430025743	,	-0.321943379	,	-1.328351677	,	0.436149252	,	0.014307793	,	1.178377037	,	-0.116573859	,	0.584179965	},
{	0.438282592	,	-1.183630704	,	-0.280667451	,	-0.826440203	,	-0.876958073	,	0.853225689	,	-1.203952736	,	0.717522549	,	0.289921155	,	-0.497135798	,	0.465726512	,	-1.156542515	,	-0.701916911	,	0.129584728	,	0.306784851	,	-0.349747409	,	-0.361217549	,	-0.21816011	,	0.838258177	,	0.726120682	},
{	-0.31996706	,	0.920116913	,	0.480549786	,	0.769544265	,	0.765288945	,	-0.30212785	,	0.477937232	,	0.851937087	,	-0.455548093	,	-0.1835929	,	-0.131589058	,	0.408135812	,	-0.63689119	,	-0.333778295	,	-0.390201459	,	-0.944198171	,	-0.093542667	,	0.146691221	,	1.04138973	,	-0.526667288	},
{	0.23527534	,	-0.806097504	,	0.257380711	,	-1.463758247	,	-0.749871399	,	1.13305227	,	-1.239715315	,	-0.161876127	,	-0.268341548	,	-1.112370684	,	0.656147668	,	-1.032644	,	0.532553276	,	-1.049639618	,	0.057107994	,	-0.097741737	,	-0.149346678	,	-0.136350957	,	-0.815015315	,	1.160391508	},
{	0.099217539	,	0.399974666	,	-0.370195351	,	-0.02673917	,	-0.741936133	,	1.102889096	,	0.367527871	,	0.005645884	,	0.659975853	,	-0.291657868	,	-1.146985079	,	-0.863317263	,	-0.592437702	,	-1.19721251	,	0.714223113	,	0.340474219	,	0.483831114	,	-1.198098953	,	0.098210174	,	0.478768203	},
{	1.240736147	,	-0.183969279	,	-0.343008605	,	0.664288858	,	-0.608199378	,	0.01185938	,	0.035902009	,	-1.139262864	,	-0.254956831	,	-0.819893196	,	1.216399006	,	-0.176602788	,	-0.469711713	,	1.076767311	,	-0.528962789	,	-1.305191304	,	-0.195111153	,	0.417569104	,	-1.594337043	,	0.409072148	},
{	0.869907151	,	-0.122941513	,	-0.490769121	,	0.653670752	,	0.91715162	,	0.339788127	,	0.792165484	,	1.058124587	,	-0.104276023	,	-0.208376873	,	0.77176294	,	0.421353102	,	0.583595806	,	0.195906458	,	1.268513773	,	-0.62624241	,	-0.145678645	,	0.153837499	,	0.621972545	,	0.704056707	},
{	-1.48943248	,	-0.662830685	,	-0.662927003	,	-0.981349257	,	-0.987529246	,	0.707911253	,	0.009194627	,	0.242889444	,	-0.080897465	,	0.173405155	,	-0.346303119	,	0.169834927	,	0.525374939	,	-1.213533594	,	0.930356534	,	-1.313755012	,	-0.460936746	,	-0.60215879	,	1.13450299	,	-0.614464781	},
{	1.070586945	,	0.730499803	,	-0.084162866	,	0.380877149	,	-0.416596789	,	-0.262468625	,	-0.35713178	,	-0.079519626	,	1.034489627	,	-0.214975331	,	-0.302857154	,	-0.952338716	,	-0.013144745	,	-0.533041659	,	0.109094575	,	0.634608685	,	-0.105673879	,	0.618151311	,	0.797052761	,	0.377416565	},
{	0.979521186	,	0.39859514	,	-0.264816988	,	0.654409335	,	-1.002852052	,	-0.914653233	,	0.258316806	,	-0.736457972	,	0.025227601	,	0.061582411	,	0.461728664	,	-0.52858126	,	-1.138592519	,	0.776218242	,	-1.161210491	,	-0.570901215	,	-0.682695306	,	0.725039783	,	-0.674084562	,	-0.971387207	},
{	1.163335547	,	-0.251808137	,	0.408820629	,	-0.383724115	,	-0.543584189	,	-0.445101822	,	-0.604358538	,	-0.117834497	,	-0.274895332	,	0.927516036	,	0.567595356	,	-0.011907333	,	-0.219367572	,	0.105253207	,	0.467913742	,	-2.215853369	,	-0.105450832	,	-0.165999741	,	0.28706175	,	-1.024726991	},
{	0.204186465	,	0.032878121	,	-0.594933756	,	1.224655404	,	-0.056217084	,	0.052238574	,	1.414003777	,	-0.639693066	,	-0.526795171	,	0.84406776	,	-0.962841625	,	0.662816321	,	-0.408879772	,	-0.87525364	,	-1.36712876	,	0.530412621	,	-0.74544136	,	1.205766276	,	-0.914394969	,	-0.14231785	},
{	1.432218114	,	-0.652179141	,	-0.061339937	,	-0.239704755	,	-0.548502948	,	0.215849277	,	0.650191331	,	-1.014650737	,	-0.46050508	,	-1.215830677	,	1.163285489	,	-0.262577014	,	-1.078854401	,	0.647216369	,	-0.522883075	,	0.055493825	,	-0.245133654	,	-0.255996918	,	-1.606028383	,	0.681132774	},
{	-0.770512707	,	1.074623674	,	-1.501982482	,	-0.639906579	,	-0.956096817	,	-1.194510344	,	-0.065962047	,	0.409407422	,	-0.635401788	,	0.422206024	,	-0.749788257	,	-1.06422967	,	0.903277559	,	-0.310289105	,	0.83073653	,	-0.931118024	,	-0.041759255	,	-0.777695898	,	-0.302745317	,	-1.352137893	},
{	0.809934164	,	-1.770343794	,	0.682612702	,	-0.131314869	,	-0.491020867	,	1.107897781	,	-0.82160769	,	0.294128826	,	-0.829288211	,	-1.616346424	,	0.363850666	,	-0.703059933	,	0.095230599	,	-1.078748418	,	0.277570826	,	0.447396619	,	-0.506403865	,	-0.128037094	,	0.566066672	,	1.177562752	},
{	0.726947078	,	0.36201146	,	0.151324569	,	-0.519810678	,	-0.15008089	,	-0.659156727	,	0.138623839	,	-1.24212505	,	-0.17397238	,	-0.34619782	,	0.638530381	,	0.433668442	,	-0.951322504	,	1.156629077	,	-1.191922232	,	-1.515967872	,	-0.219128327	,	0.540630714	,	-0.94410315	,	-0.047211387	},
{	-0.160714356	,	0.00135393	,	-0.767642659	,	-0.439996659	,	1.147628198	,	-0.893015954	,	0.395234679	,	1.245381182	,	-0.375376468	,	0.042471238	,	-0.993083601	,	-0.71132884	,	0.116154481	,	0.533883178	,	-0.234148249	,	-0.004824171	,	-0.467230675	,	0.442877013	,	0.266522727	,	-0.690938774	},
{	-1.037042133	,	-0.092828072	,	-1.283367219	,	-0.290752539	,	-0.641169619	,	-0.286205169	,	-0.906506045	,	0.547335835	,	0.265848359	,	-0.552821407	,	-1.167140954	,	-0.863538734	,	0.326979667	,	0.126959484	,	0.629060799	,	-0.909777867	,	0.057539214	,	-1.096035732	,	-0.261244195	,	-0.13014221	},
{	0.572205536	,	0.719359032	,	-0.065864868	,	1.513835962	,	-0.234838238	,	-1.047077292	,	0.11977757	,	-0.715335122	,	-0.730643044	,	0.8505301	,	-1.289469554	,	0.840284973	,	-0.916125848	,	1.181062383	,	-0.557511217	,	0.863575448	,	-0.545317674	,	0.576502372	,	-0.475700003	,	-0.1338282	},
{	-1.534865978	,	-0.945373082	,	-0.733261451	,	0.690908873	,	0.741254313	,	0.665559294	,	0.149535426	,	1.459348392	,	-0.441578917	,	-0.365371864	,	-0.613790982	,	0.835650882	,	0.810847857	,	-0.780023042	,	0.06777552	,	0.688998436	,	-0.541606835	,	-0.554928775	,	1.367866709	,	-0.780513203	}
    

};

float w3[20][20] = {
{	-0.369244543	,	1.704452623	,	-0.467305619	,	1.575308464	,	-0.299123275	,	-0.6787113	,	-0.55544238	,	0.206462137	,	-0.051294422	,	-1.19622862	,	0.995922382	,	-1.053768362	,	-0.414871548	,	0.769492124	,	1.452998893	,	-1.151077214	,	0.373014382	,	0.294613024	,	-1.556938391	,	-0.241774275	},
{	-1.072162804	,	-1.76718282	,	0.140853666	,	-1.551519314	,	-0.501588972	,	0.876294342	,	-0.321359467	,	-1.689813127	,	0.209377426	,	0.242165939	,	-0.226412807	,	0.981153255	,	0.543917312	,	0.198569652	,	-0.505253058	,	-0.171306742	,	0.625682475	,	-0.808538374	,	0.939975704	,	-0.587396573	},
{	-2.090536783	,	-0.790155645	,	0.107894794	,	-1.523117455	,	0.292199404	,	-0.410348334	,	-0.225200514	,	0.513694728	,	-0.557719937	,	-0.068362124	,	0.598668115	,	-0.739330226	,	-2.304995796	,	1.249475526	,	-1.35013175	,	-0.151171853	,	1.044085711	,	0.612204098	,	-0.13151174	,	-0.244077282	},
{	-1.760535399	,	-0.233486585	,	-0.571901923	,	-0.598286314	,	-0.577933394	,	-1.306833579	,	-0.533675683	,	1.48316652	,	-0.252056248	,	-0.372803042	,	1.173165477	,	-1.241282438	,	-1.360379211	,	1.524305325	,	-0.733830286	,	0.181912779	,	0.592831764	,	0.902775242	,	-0.509581984	,	-0.475040095	},
{	-0.91778295	,	-0.85948448	,	-0.050330195	,	0.416688122	,	0.4319922	,	-0.802366479	,	0.094870562	,	0.097186476	,	-0.357793847	,	-1.506911415	,	-1.385464538	,	0.115019915	,	0.020465316	,	1.156907666	,	0.12531924	,	-1.224118067	,	0.874355748	,	0.412886292	,	-0.364510744	,	1.713715569	},
{	0.026817434	,	0.161069802	,	0.709222429	,	-0.720670678	,	-0.518685969	,	0.381971008	,	-0.955085858	,	-0.409259521	,	-0.695989906	,	1.211939033	,	0.256563703	,	0.586628906	,	-0.575064875	,	-0.353048061	,	-1.346236904	,	0.722127472	,	-0.572906606	,	-0.41664494	,	-0.060240813	,	0.03204594	},
{	-0.663025139	,	0.301785325	,	0.678691618	,	0.781534835	,	-0.16729492	,	0.400909616	,	0.724552328	,	-1.166332847	,	-0.924081994	,	0.60399176	,	-0.404575188	,	-0.732856207	,	0.302152113	,	-0.604975791	,	0.151713532	,	-0.557180072	,	-0.481468793	,	-0.422988366	,	0.257098406	,	-0.040670779	},
{	0.521931611	,	-0.261585648	,	-0.824549297	,	0.049591257	,	-0.72002426	,	1.476503808	,	-0.46094598	,	-1.388277918	,	-0.389898301	,	1.087634536	,	0.273856374	,	0.403353309	,	1.930888849	,	-0.373715042	,	-0.462236306	,	1.146193657	,	-0.250278884	,	-0.309700611	,	-0.52202301	,	-1.469818389	},
{	0.563010239	,	1.087073862	,	-0.097325501	,	0.282708327	,	0.504835364	,	-0.432458071	,	-0.676479336	,	-0.116513828	,	-0.103009618	,	-1.276708235	,	-0.271987805	,	-1.430227018	,	0.737191635	,	-0.165714168	,	0.791883417	,	-1.082010586	,	-1.046287894	,	0.184836027	,	-1.292726271	,	0.220766002	},
{	-0.949682862	,	0.720388512	,	0.092133162	,	0.526259933	,	0.387776458	,	0.328380911	,	0.386650804	,	0.902381227	,	0.580870665	,	-0.367183647	,	0.526587239	,	1.01895254	,	1.034327548	,	0.886768595	,	-0.592274414	,	-0.568268298	,	0.539122634	,	-0.629994533	,	0.902232808	,	0.195713167	},
{	-0.824118996	,	-0.297633068	,	-0.150004648	,	0.278213158	,	-0.186921625	,	1.360716578	,	-0.440583127	,	-0.428750682	,	-0.356621868	,	0.732788115	,	0.605913617	,	-0.375976089	,	0.394324097	,	1.206995995	,	-0.691224518	,	1.314031646	,	-0.690007641	,	0.92122309	,	-0.075596837	,	-0.681314196	},
{	0.135433838	,	-0.469883866	,	0.620350042	,	1.220055672	,	1.093691082	,	-0.657476953	,	0.350364088	,	1.200282832	,	0.780842776	,	0.692493759	,	0.012627564	,	-0.849975888	,	0.687815881	,	0.984322418	,	1.156414909	,	0.690800272	,	1.110791548	,	1.035279115	,	-0.052897743	,	0.381607044	},
{	0.732597842	,	0.032848154	,	-0.705579522	,	0.892081877	,	-0.205029805	,	-0.296572839	,	-0.169993418	,	-0.780520055	,	0.20878076	,	-0.863157868	,	-0.783441503	,	-1.246298955	,	0.946595725	,	-0.798295994	,	0.959486824	,	-0.580236663	,	-0.646626405	,	0.01404791	,	-0.166391358	,	-0.547053704	},
{	1.084091049	,	1.185215592	,	0.169509403	,	0.425057267	,	0.571853206	,	-0.725100262	,	0.740698944	,	0.941231169	,	1.064671971	,	0.165215429	,	0.806289423	,	0.588354822	,	0.347216617	,	1.071183336	,	-0.375602246	,	-0.27740306	,	-0.223668673	,	-0.553003007	,	0.417712671	,	1.284113983	},
{	0.527368462	,	0.295701095	,	-0.728589286	,	1.222832111	,	0.35932273	,	-0.409534972	,	-0.526387785	,	0.19035058	,	-0.259042748	,	-1.348668894	,	0.365313157	,	-1.374915182	,	0.337563693	,	-0.090747989	,	1.195749932	,	0.165828993	,	-0.625979117	,	0.131879566	,	-0.977157144	,	-0.08330959	},
{	0.093212528	,	1.16608392	,	-0.652000775	,	0.999441042	,	-0.495309489	,	-0.208602565	,	-0.826707221	,	0.269174056	,	-0.285167585	,	-0.311774557	,	-0.551416414	,	-0.807038275	,	1.476387237	,	-1.035277714	,	1.261431555	,	-0.878487669	,	-0.61602733	,	0.119745353	,	-1.187858889	,	0.345155285	},
{	-0.537666729	,	0.167009122	,	0.239510865	,	-0.338791771	,	-0.165497139	,	-1.297249084	,	-0.284087743	,	-0.077971324	,	0.559152452	,	-0.819917345	,	-1.21506518	,	0.562316468	,	-1.237912888	,	0.5380599	,	-0.092679348	,	-1.187357797	,	-0.047492212	,	-0.344445026	,	0.115490788	,	1.154240171	},
{	-0.836092793	,	-1.458121389	,	-0.405825733	,	-0.929839209	,	0.221142822	,	0.528787762	,	-0.140610617	,	-0.962492702	,	-0.284551558	,	0.693350798	,	-0.613688636	,	0.797353491	,	0.986188743	,	-0.183679862	,	-0.880331859	,	1.080385058	,	-0.40821208	,	-0.718087844	,	1.105878101	,	-0.219024577	},
{	-1.54002751	,	0.43045258	,	-0.238162257	,	-1.445476816	,	-0.417769888	,	-1.222155772	,	-0.252863479	,	0.251794301	,	-0.167020527	,	-0.491473842	,	0.207523118	,	0.444697919	,	-0.633104673	,	1.001723323	,	-0.302185874	,	-1.41225673	,	-0.025868395	,	0.955047873	,	0.160323189	,	0.712843829	},
{	-0.403411734	,	-0.687769468	,	0.036274779	,	0.274721353	,	0.332560674	,	-1.814420679	,	0.01981486	,	1.135952309	,	-0.52144983	,	-1.444065747	,	-1.062335151	,	0.575328695	,	-1.448990588	,	0.032236418	,	0.350279467	,	-1.760990658	,	0.437464508	,	0.663208095	,	0.343280998	,	1.463340576	}

};

float w4[5][20] = {
{	-1.757729078	,	1.623088351	,	1.40809065	,	-0.541800153	,	1.293510493	,	0.901830971	,	-0.158682895	,	-0.534554708	,	-1.371215306	,	-0.459907415	,	-0.428277477	,	-0.50066957	,	-0.932453858	,	0.123169811	,	-1.580521594	,	-1.705724851	,	1.349733353	,	1.422758171	,	1.297722263	,	1.124339636	},
{	1.048122629	,	-0.997350057	,	-2.16995024	,	-1.271902975	,	-0.660871799	,	-0.66284198	,	0.086256024	,	1.289399391	,	1.264001026	,	-0.406918792	,	0.182824038	,	-0.30720111	,	1.482881246	,	0.20605671	,	1.5522451	,	1.481989176	,	-2.654887966	,	-0.23422358	,	-1.674057518	,	-1.47509205	},
{	1.399859434	,	-1.859188463	,	-1.628242745	,	-1.004315497	,	1.13314751	,	-0.097885653	,	-0.183141051	,	-2.116895676	,	1.472399308	,	-0.552730951	,	-2.421799949	,	-0.341786991	,	1.013663689	,	0.163992722	,	1.490190749	,	1.363317799	,	0.034664486	,	-1.483347894	,	0.096229466	,	1.407609561	},
{	-2.220261354	,	1.809753675	,	-0.182789353	,	-0.929873758	,	-2.389955428	,	2.105661348	,	-0.003498428	,	1.985682797	,	-2.084657325	,	-0.251558502	,	1.099106209	,	-0.299362608	,	-1.267876472	,	0.303424933	,	-1.064444863	,	-1.615662222	,	-1.395251456	,	1.958578282	,	-1.020480969	,	-2.764149224	},
{	1.361925754	,	-1.486959745	,	2.653534968	,	2.952425	,	1.208439311	,	-0.571796305	,	-1.060451134	,	-2.719734821	,	0.185657686	,	-0.588369946	,	0.435042323	,	-0.431731082	,	-1.723437046	,	-0.042889432	,	0.104036018	,	-0.691852212	,	0.107133877	,	-1.868581728	,	2.079483507	,	0.881982832	}
};


float x1[25] = {
0,
0,
0,
0,
0,
1,
0,
0,
0,
1,
1,
1,
1,
1,
1,
0,
0,
0,
0,
1,
0,
0,
0,
0,
0,
};

float x2[25] = {
    1	,
0	,
0	,
1	,
1	,
1	,
0	,
1	,
0	,
1	,
1	,
0	,
1	,
0	,
1	,
1	,
0	,
1	,
0	,
1	,
0	,
1	,
0	,
0	,
1	,

};
float x3[25] = {
1	,
0	,
0	,
0	,
1	,
1	,
0	,
1	,
0	,
1	,
1	,
0	,
1	,
0	,
1	,
1	,
0	,
1	,
0	,
1	,
0	,
1	,
0	,
1	,
0	,
};
float x4[25] = {
0	,
0	,
0	,
1	,
0	,
0	,
0	,
1	,
1	,
0	,
0	,
1	,
0	,
1	,
0	,
1	,
1	,
1	,
1	,
1	,
0	,
0	,
0	,
1	,
0	,

};
float x5[25] = {
1	,
1	,
1	,
0	,
0	,
1	,
0	,
1	,
0	,
1	,
1	,
0	,
1	,
0	,
1	,
1	,
0	,
1	,
0	,
1	,
1	,
0	,
0	,
1	,
0	,

};
