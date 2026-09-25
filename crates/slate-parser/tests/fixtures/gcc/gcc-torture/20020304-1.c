// SLATE-FILECHECK-DEFINES DEFAULT

/* In 3.0, this test case (extracted from Bigloo) crashes the compiler in
   bb-reorder.c.  This is a regression from 2.95, already fixed in 3.1.

   Original bug report is c/5830 by Manuel Serrano <Manuel.Serrano@inria.fr>.
 */

/* { dg-require-stack-size "513" } */

typedef union scmobj {
  struct pair {
    union scmobj *car;
    union scmobj *cdr;
  } pair_t;
  struct vector {
    long header;
    int length;
    union scmobj *obj0;
  } vector_t;
} *obj_t;

extern obj_t create_vector (int);
extern obj_t make_pair (obj_t, obj_t);
extern long bgl_list_length (obj_t);
extern int BGl_equalzf3zf3zz__r4_equivalence_6_2z00 (obj_t, obj_t);
extern obj_t BGl_evcompilezd2lambdazd2zz__evcompilez00 (obj_t
							BgL_formalsz00_39,
							obj_t BgL_bodyz00_40,
							obj_t BgL_wherez00_41,
							obj_t
							BgL_namedzf3zf3_42,
							obj_t BgL_locz00_43);

obj_t
BGl_evcompilezd2lambdazd2zz__evcompilez00 (obj_t BgL_formalsz00_39,
					   obj_t BgL_bodyz00_40,
					   obj_t BgL_wherez00_41,
					   obj_t BgL_namedzf3zf3_42,
					   obj_t BgL_locz00_43)
{
  if (BGl_equalzf3zf3zz__r4_equivalence_6_2z00
      (BgL_formalsz00_39,
       ((obj_t) (obj_t) ((long) (((long) (0) << 2) | 2))))) {
  BgL_tagzd21966zd2_943:
    if ((BgL_namedzf3zf3_42 !=
	 ((obj_t) (obj_t) ((long) (((long) (1) << 2) | 2))))) {
      obj_t BgL_v1042z00_998;
      {
	int BgL_auxz00_4066;
	BgL_auxz00_4066 = (int) (((long) 3));
	BgL_v1042z00_998 = create_vector (BgL_auxz00_4066);
      }
      {
	obj_t BgL_arg1586z00_1000;
	BgL_arg1586z00_1000 = make_pair (BgL_wherez00_41, BgL_bodyz00_40);
	{
	  int BgL_auxz00_4070;
	  BgL_auxz00_4070 = (int) (((long) 2));
	  ((&(((obj_t) (BgL_v1042z00_998))->vector_t.obj0))[BgL_auxz00_4070] =
	   BgL_arg1586z00_1000,
	   ((obj_t) (obj_t) ((long) (((long) (3) << 2) | 2))));
	}
      }
      {
	int BgL_auxz00_4073;
	BgL_auxz00_4073 = (int) (((long) 1));
	((&(((obj_t) (BgL_v1042z00_998))->vector_t.obj0))[BgL_auxz00_4073] =
	 BgL_locz00_43, ((obj_t) (obj_t) ((long) (((long) (3) << 2) | 2))));
      }
      {
	obj_t BgL_auxz00_4078;
	int BgL_auxz00_4076;
	{
	  long BgL_auxz00_4079;
	  {
	    long BgL_auxz00_4080;
	    BgL_auxz00_4080 = bgl_list_length (BgL_formalsz00_39);
	    BgL_auxz00_4079 = (BgL_auxz00_4080 + ((long) 37));
	  }
	  BgL_auxz00_4078 =
	    (obj_t) ((long) (((long) (BgL_auxz00_4079) << 2) | 1));
	}
	BgL_auxz00_4076 = (int) (((long) 0));
	((&(((obj_t) (BgL_v1042z00_998))->vector_t.obj0))[BgL_auxz00_4076] =
	 BgL_auxz00_4078, ((obj_t) (obj_t) ((long) (((long) (3) << 2) | 2))));
      }
      return BgL_v1042z00_998;
    } else {
      obj_t BgL_v1043z00_1005;
      {
	int BgL_auxz00_4085;
	BgL_auxz00_4085 = (int) (((long) 3));
	BgL_v1043z00_1005 = create_vector (BgL_auxz00_4085);
      }
      {
	int BgL_auxz00_4088;
	BgL_auxz00_4088 = (int) (((long) 2));
	((&(((obj_t) (BgL_v1043z00_1005))->vector_t.obj0))[BgL_auxz00_4088] =
	 BgL_bodyz00_40, ((obj_t) (obj_t) ((long) (((long) (3) << 2) | 2))));
      }
      {
	int BgL_auxz00_4091;
	BgL_auxz00_4091 = (int) (((long) 1));
	((&(((obj_t) (BgL_v1043z00_1005))->vector_t.obj0))[BgL_auxz00_4091] =
	 BgL_locz00_43, ((obj_t) (obj_t) ((long) (((long) (3) << 2) | 2))));
      }
      {
	obj_t BgL_auxz00_4096;
	int BgL_auxz00_4094;
	{
	  long BgL_auxz00_4097;
	  {
	    long BgL_auxz00_4098;
	    BgL_auxz00_4098 = bgl_list_length (BgL_formalsz00_39);
	    BgL_auxz00_4097 = (BgL_auxz00_4098 + ((long) 42));
	  }
	  BgL_auxz00_4096 =
	    (obj_t) ((long) (((long) (BgL_auxz00_4097) << 2) | 1));
	}
	BgL_auxz00_4094 = (int) (((long) 0));
	((&(((obj_t) (BgL_v1043z00_1005))->vector_t.obj0))[BgL_auxz00_4094] =
	 BgL_auxz00_4096, ((obj_t) (obj_t) ((long) (((long) (3) << 2) | 2))));
      }
      return BgL_v1043z00_1005;
    }
  } else {
    if (((((long) BgL_formalsz00_39) & ((1 << 2) - 1)) == 3)) {
      if (BGl_equalzf3zf3zz__r4_equivalence_6_2z00
	  (((((obj_t) ((long) BgL_formalsz00_39 - 3))->pair_t).cdr),
	   ((obj_t) (obj_t) ((long) (((long) (0) << 2) | 2))))) {
	goto BgL_tagzd21966zd2_943;
      } else {
	obj_t BgL_cdrzd21979zd2_953;
	BgL_cdrzd21979zd2_953 =
	  ((((obj_t) ((long) BgL_formalsz00_39 - 3))->pair_t).cdr);
	if (((((long) BgL_cdrzd21979zd2_953) & ((1 << 2) - 1)) == 3)) {
	  if (BGl_equalzf3zf3zz__r4_equivalence_6_2z00
	      (((((obj_t) ((long) BgL_cdrzd21979zd2_953 - 3))->pair_t).cdr),
	       ((obj_t) (obj_t) ((long) (((long) (0) << 2) | 2))))) {
	    goto BgL_tagzd21966zd2_943;
	  } else {
	    obj_t BgL_cdrzd21986zd2_956;
	    BgL_cdrzd21986zd2_956 =
	      ((((obj_t) ((long) BgL_cdrzd21979zd2_953 - 3))->pair_t).cdr);
	    if (((((long) BgL_cdrzd21986zd2_956) & ((1 << 2) - 1)) == 3)) {
	      if (BGl_equalzf3zf3zz__r4_equivalence_6_2z00
		  (((((obj_t) ((long) BgL_cdrzd21986zd2_956 - 3))->pair_t).
		    cdr),
		   ((obj_t) (obj_t) ((long) (((long) (0) << 2) | 2))))) {
		goto BgL_tagzd21966zd2_943;
	      } else {
		obj_t BgL_cdrzd21994zd2_959;
		{
		  obj_t BgL_auxz00_4120;
		  BgL_auxz00_4120 =
		    ((((obj_t) ((long) BgL_cdrzd21979zd2_953 - 3))->pair_t).
		     cdr);
		  BgL_cdrzd21994zd2_959 =
		    ((((obj_t) ((long) BgL_auxz00_4120 - 3))->pair_t).cdr);
		}
		if (((((long) BgL_cdrzd21994zd2_959) & ((1 << 2) - 1)) == 3)) {
		  if (BGl_equalzf3zf3zz__r4_equivalence_6_2z00
		      (((((obj_t) ((long) BgL_cdrzd21994zd2_959 - 3))->
			 pair_t).cdr),
		       ((obj_t) (obj_t) ((long) (((long) (0) << 2) | 2))))) {
		    goto BgL_tagzd21966zd2_943;
		  } else {
		    int BgL_testz00_4128;
		    {
		      obj_t BgL_auxz00_4129;
		      BgL_auxz00_4129 =
			((((obj_t) ((long) BgL_formalsz00_39 - 3))->pair_t).
			 car);
		      BgL_testz00_4128 =
			((((long) BgL_auxz00_4129) & ((1 << 2) - 1)) == 3);
		    }
		    if (BgL_testz00_4128) {
		    BgL_tagzd21971zd2_948:
		      if ((BgL_namedzf3zf3_42 !=
			   ((obj_t) (obj_t)
			    ((long) (((long) (1) << 2) | 2))))) {
			obj_t BgL_v1052z00_1026;
			{
			  int BgL_auxz00_4134;
			  BgL_auxz00_4134 = (int) (((long) 3));
			  BgL_v1052z00_1026 = create_vector (BgL_auxz00_4134);
			}
			{
			  obj_t BgL_arg1606z00_1028;
			  {
			    obj_t BgL_v1053z00_1029;
			    {
			      int BgL_auxz00_4137;
			      BgL_auxz00_4137 = (int) (((long) 3));
			      BgL_v1053z00_1029 =
				create_vector (BgL_auxz00_4137);
			    }
			    {
			      int BgL_auxz00_4140;
			      BgL_auxz00_4140 = (int) (((long) 2));
			      ((&
				(((obj_t) (BgL_v1053z00_1029))->vector_t.
				 obj0))[BgL_auxz00_4140] =
			       BgL_formalsz00_39,
			       ((obj_t) (obj_t)
				((long) (((long) (3) << 2) | 2))));
			    }
			    {
			      int BgL_auxz00_4143;
			      BgL_auxz00_4143 = (int) (((long) 1));
			      ((&
				(((obj_t) (BgL_v1053z00_1029))->vector_t.
				 obj0))[BgL_auxz00_4143] =
			       BgL_bodyz00_40,
			       ((obj_t) (obj_t)
				((long) (((long) (3) << 2) | 2))));
			    }
			    {
			      int BgL_auxz00_4146;
			      BgL_auxz00_4146 = (int) (((long) 0));
			      ((&
				(((obj_t) (BgL_v1053z00_1029))->vector_t.
				 obj0))[BgL_auxz00_4146] =
			       BgL_wherez00_41,
			       ((obj_t) (obj_t)
				((long) (((long) (3) << 2) | 2))));
			    }
			    BgL_arg1606z00_1028 = BgL_v1053z00_1029;
			  }
			  {
			    int BgL_auxz00_4149;
			    BgL_auxz00_4149 = (int) (((long) 2));
			    ((&(((obj_t) (BgL_v1052z00_1026))->vector_t.obj0))
			     [BgL_auxz00_4149] =
			     BgL_arg1606z00_1028,
			     ((obj_t) (obj_t)
			      ((long) (((long) (3) << 2) | 2))));
			  }
			}
			{
			  int BgL_auxz00_4152;
			  BgL_auxz00_4152 = (int) (((long) 1));
			  ((&(((obj_t) (BgL_v1052z00_1026))->vector_t.obj0))
			   [BgL_auxz00_4152] =
			   BgL_locz00_43,
			   ((obj_t) (obj_t)
			    ((long) (((long) (3) << 2) | 2))));
			}
			{
			  obj_t BgL_auxz00_4157;
			  int BgL_auxz00_4155;
			  BgL_auxz00_4157 =
			    (obj_t) ((long)
				     (((long) (((long) 55)) << 2) | 1));
			  BgL_auxz00_4155 = (int) (((long) 0));
			  ((&(((obj_t) (BgL_v1052z00_1026))->vector_t.obj0))
			   [BgL_auxz00_4155] =
			   BgL_auxz00_4157,
			   ((obj_t) (obj_t)
			    ((long) (((long) (3) << 2) | 2))));
			}
			return BgL_v1052z00_1026;
		      } else {
			obj_t BgL_v1054z00_1030;
			{
			  int BgL_auxz00_4160;
			  BgL_auxz00_4160 = (int) (((long) 3));
			  BgL_v1054z00_1030 = create_vector (BgL_auxz00_4160);
			}
			{
			  obj_t BgL_arg1608z00_1032;
			  BgL_arg1608z00_1032 =
			    make_pair (BgL_bodyz00_40, BgL_formalsz00_39);
			  {
			    int BgL_auxz00_4164;
			    BgL_auxz00_4164 = (int) (((long) 2));
			    ((&(((obj_t) (BgL_v1054z00_1030))->vector_t.obj0))
			     [BgL_auxz00_4164] =
			     BgL_arg1608z00_1032,
			     ((obj_t) (obj_t)
			      ((long) (((long) (3) << 2) | 2))));
			  }
			}
			{
			  int BgL_auxz00_4167;
			  BgL_auxz00_4167 = (int) (((long) 1));
			  ((&(((obj_t) (BgL_v1054z00_1030))->vector_t.obj0))
			   [BgL_auxz00_4167] =
			   BgL_locz00_43,
			   ((obj_t) (obj_t)
			    ((long) (((long) (3) << 2) | 2))));
			}
			{
			  obj_t BgL_auxz00_4172;
			  int BgL_auxz00_4170;
			  BgL_auxz00_4172 =
			    (obj_t) ((long)
				     (((long) (((long) 56)) << 2) | 1));
			  BgL_auxz00_4170 = (int) (((long) 0));
			  ((&(((obj_t) (BgL_v1054z00_1030))->vector_t.obj0))
			   [BgL_auxz00_4170] =
			   BgL_auxz00_4172,
			   ((obj_t) (obj_t)
			    ((long) (((long) (3) << 2) | 2))));
			}
			return BgL_v1054z00_1030;
		      }
		    } else {
		      int BgL_testz00_4175;
		      {
			obj_t BgL_auxz00_4176;
			{
			  obj_t BgL_auxz00_4177;
			  BgL_auxz00_4177 =
			    ((((obj_t) ((long) BgL_formalsz00_39 - 3))->
			      pair_t).cdr);
			  BgL_auxz00_4176 =
			    ((((obj_t) ((long) BgL_auxz00_4177 - 3))->pair_t).
			     car);
			}
			BgL_testz00_4175 =
			  ((((long) BgL_auxz00_4176) & ((1 << 2) - 1)) == 3);
		      }
		      if (BgL_testz00_4175) {
			goto BgL_tagzd21971zd2_948;
		      } else {
			int BgL_testz00_4181;
			{
			  obj_t BgL_auxz00_4182;
			  {
			    obj_t BgL_auxz00_4183;
			    {
			      obj_t BgL_auxz00_4184;
			      BgL_auxz00_4184 =
				((((obj_t) ((long) BgL_formalsz00_39 - 3))->
				  pair_t).cdr);
			      BgL_auxz00_4183 =
				((((obj_t) ((long) BgL_auxz00_4184 - 3))->
				  pair_t).cdr);
			    }
			    BgL_auxz00_4182 =
			      ((((obj_t) ((long) BgL_auxz00_4183 - 3))->
				pair_t).car);
			  }
			  BgL_testz00_4181 =
			    ((((long) BgL_auxz00_4182) & ((1 << 2) - 1)) ==
			     3);
			}
			if (BgL_testz00_4181) {
			  goto BgL_tagzd21971zd2_948;
			} else {
			  goto BgL_tagzd21971zd2_948;
			}
		      }
		    }
		  }
		} else {
		  int BgL_testz00_4189;
		  {
		    obj_t BgL_auxz00_4190;
		    BgL_auxz00_4190 =
		      ((((obj_t) ((long) BgL_formalsz00_39 - 3))->pair_t).
		       car);
		    BgL_testz00_4189 =
		      ((((long) BgL_auxz00_4190) & ((1 << 2) - 1)) == 3);
		  }
		  if (BgL_testz00_4189) {
		    goto BgL_tagzd21971zd2_948;
		  } else {
		    int BgL_testz00_4193;
		    {
		      obj_t BgL_auxz00_4194;
		      {
			obj_t BgL_auxz00_4195;
			BgL_auxz00_4195 =
			  ((((obj_t) ((long) BgL_formalsz00_39 - 3))->pair_t).
			   cdr);
			BgL_auxz00_4194 =
			  ((((obj_t) ((long) BgL_auxz00_4195 - 3))->pair_t).
			   car);
		      }
		      BgL_testz00_4193 =
			((((long) BgL_auxz00_4194) & ((1 << 2) - 1)) == 3);
		    }
		    if (BgL_testz00_4193) {
		      goto BgL_tagzd21971zd2_948;
		    } else {
		      int BgL_testz00_4199;
		      {
			obj_t BgL_auxz00_4200;
			{
			  obj_t BgL_auxz00_4201;
			  {
			    obj_t BgL_auxz00_4202;
			    BgL_auxz00_4202 =
			      ((((obj_t) ((long) BgL_formalsz00_39 - 3))->
				pair_t).cdr);
			    BgL_auxz00_4201 =
			      ((((obj_t) ((long) BgL_auxz00_4202 - 3))->
				pair_t).cdr);
			  }
			  BgL_auxz00_4200 =
			    ((((obj_t) ((long) BgL_auxz00_4201 - 3))->pair_t).
			     car);
			}
			BgL_testz00_4199 =
			  ((((long) BgL_auxz00_4200) & ((1 << 2) - 1)) == 3);
		      }
		      if (BgL_testz00_4199) {
			goto BgL_tagzd21971zd2_948;
		      } else {
			if ((BgL_namedzf3zf3_42 !=
			     ((obj_t) (obj_t)
			      ((long) (((long) (1) << 2) | 2))))) {
			  obj_t BgL_v1050z00_1022;
			  {
			    int BgL_auxz00_4209;
			    BgL_auxz00_4209 = (int) (((long) 3));
			    BgL_v1050z00_1022 =
			      create_vector (BgL_auxz00_4209);
			  }
			  {
			    obj_t BgL_arg1604z00_1024;
			    BgL_arg1604z00_1024 =
			      make_pair (BgL_wherez00_41, BgL_bodyz00_40);
			    {
			      int BgL_auxz00_4213;
			      BgL_auxz00_4213 = (int) (((long) 2));
			      ((&
				(((obj_t) (BgL_v1050z00_1022))->vector_t.
				 obj0))[BgL_auxz00_4213] =
			       BgL_arg1604z00_1024,
			       ((obj_t) (obj_t)
				((long) (((long) (3) << 2) | 2))));
			    }
			  }
			  {
			    int BgL_auxz00_4216;
			    BgL_auxz00_4216 = (int) (((long) 1));
			    ((&(((obj_t) (BgL_v1050z00_1022))->vector_t.obj0))
			     [BgL_auxz00_4216] =
			     BgL_locz00_43,
			     ((obj_t) (obj_t)
			      ((long) (((long) (3) << 2) | 2))));
			  }
			  {
			    obj_t BgL_auxz00_4221;
			    int BgL_auxz00_4219;
			    BgL_auxz00_4221 =
			      (obj_t) ((long)
				       (((long) (((long) 50)) << 2) | 1));
			    BgL_auxz00_4219 = (int) (((long) 0));
			    ((&(((obj_t) (BgL_v1050z00_1022))->vector_t.obj0))
			     [BgL_auxz00_4219] =
			     BgL_auxz00_4221,
			     ((obj_t) (obj_t)
			      ((long) (((long) (3) << 2) | 2))));
			  }
			  return BgL_v1050z00_1022;
			} else {
			  obj_t BgL_v1051z00_1025;
			  {
			    int BgL_auxz00_4224;
			    BgL_auxz00_4224 = (int) (((long) 3));
			    BgL_v1051z00_1025 =
			      create_vector (BgL_auxz00_4224);
			  }
			  {
			    int BgL_auxz00_4227;
			    BgL_auxz00_4227 = (int) (((long) 2));
			    ((&(((obj_t) (BgL_v1051z00_1025))->vector_t.obj0))
			     [BgL_auxz00_4227] =
			     BgL_bodyz00_40,
			     ((obj_t) (obj_t)
			      ((long) (((long) (3) << 2) | 2))));
			  }
			  {
			    int BgL_auxz00_4230;
			    BgL_auxz00_4230 = (int) (((long) 1));
			    ((&(((obj_t) (BgL_v1051z00_1025))->vector_t.obj0))
			     [BgL_auxz00_4230] =
			     BgL_locz00_43,
			     ((obj_t) (obj_t)
			      ((long) (((long) (3) << 2) | 2))));
			  }
			  {
			    obj_t BgL_auxz00_4235;
			    int BgL_auxz00_4233;
			    BgL_auxz00_4235 =
			      (obj_t) ((long)
				       (((long) (((long) 54)) << 2) | 1));
			    BgL_auxz00_4233 = (int) (((long) 0));
			    ((&(((obj_t) (BgL_v1051z00_1025))->vector_t.obj0))
			     [BgL_auxz00_4233] =
			     BgL_auxz00_4235,
			     ((obj_t) (obj_t)
			      ((long) (((long) (3) << 2) | 2))));
			  }
			  return BgL_v1051z00_1025;
			}
		      }
		    }
		  }
		}
	      }
	    } else {
	      int BgL_testz00_4238;
	      {
		obj_t BgL_auxz00_4239;
		BgL_auxz00_4239 =
		  ((((obj_t) ((long) BgL_formalsz00_39 - 3))->pair_t).car);
		BgL_testz00_4238 =
		  ((((long) BgL_auxz00_4239) & ((1 << 2) - 1)) == 3);
	      }
	      if (BgL_testz00_4238) {
		goto BgL_tagzd21971zd2_948;
	      } else {
		int BgL_testz00_4242;
		{
		  obj_t BgL_auxz00_4243;
		  BgL_auxz00_4243 =
		    ((((obj_t) ((long) BgL_cdrzd21979zd2_953 - 3))->pair_t).
		     car);
		  BgL_testz00_4242 =
		    ((((long) BgL_auxz00_4243) & ((1 << 2) - 1)) == 3);
		}
		if (BgL_testz00_4242) {
		  goto BgL_tagzd21971zd2_948;
		} else {
		  if ((BgL_namedzf3zf3_42 !=
		       ((obj_t) (obj_t) ((long) (((long) (1) << 2) | 2))))) {
		    obj_t BgL_v1048z00_1018;
		    {
		      int BgL_auxz00_4248;
		      BgL_auxz00_4248 = (int) (((long) 3));
		      BgL_v1048z00_1018 = create_vector (BgL_auxz00_4248);
		    }
		    {
		      obj_t BgL_arg1602z00_1020;
		      BgL_arg1602z00_1020 =
			make_pair (BgL_wherez00_41, BgL_bodyz00_40);
		      {
			int BgL_auxz00_4252;
			BgL_auxz00_4252 = (int) (((long) 2));
			((&(((obj_t) (BgL_v1048z00_1018))->vector_t.obj0))
			 [BgL_auxz00_4252] =
			 BgL_arg1602z00_1020,
			 ((obj_t) (obj_t) ((long) (((long) (3) << 2) | 2))));
		      }
		    }
		    {
		      int BgL_auxz00_4255;
		      BgL_auxz00_4255 = (int) (((long) 1));
		      ((&(((obj_t) (BgL_v1048z00_1018))->vector_t.obj0))
		       [BgL_auxz00_4255] =
		       BgL_locz00_43,
		       ((obj_t) (obj_t) ((long) (((long) (3) << 2) | 2))));
		    }
		    {
		      obj_t BgL_auxz00_4260;
		      int BgL_auxz00_4258;
		      BgL_auxz00_4260 =
			(obj_t) ((long) (((long) (((long) 49)) << 2) | 1));
		      BgL_auxz00_4258 = (int) (((long) 0));
		      ((&(((obj_t) (BgL_v1048z00_1018))->vector_t.obj0))
		       [BgL_auxz00_4258] =
		       BgL_auxz00_4260,
		       ((obj_t) (obj_t) ((long) (((long) (3) << 2) | 2))));
		    }
		    return BgL_v1048z00_1018;
		  } else {
		    obj_t BgL_v1049z00_1021;
		    {
		      int BgL_auxz00_4263;
		      BgL_auxz00_4263 = (int) (((long) 3));
		      BgL_v1049z00_1021 = create_vector (BgL_auxz00_4263);
		    }
		    {
		      int BgL_auxz00_4266;
		      BgL_auxz00_4266 = (int) (((long) 2));
		      ((&(((obj_t) (BgL_v1049z00_1021))->vector_t.obj0))
		       [BgL_auxz00_4266] =
		       BgL_bodyz00_40,
		       ((obj_t) (obj_t) ((long) (((long) (3) << 2) | 2))));
		    }
		    {
		      int BgL_auxz00_4269;
		      BgL_auxz00_4269 = (int) (((long) 1));
		      ((&(((obj_t) (BgL_v1049z00_1021))->vector_t.obj0))
		       [BgL_auxz00_4269] =
		       BgL_locz00_43,
		       ((obj_t) (obj_t) ((long) (((long) (3) << 2) | 2))));
		    }
		    {
		      obj_t BgL_auxz00_4274;
		      int BgL_auxz00_4272;
		      BgL_auxz00_4274 =
			(obj_t) ((long) (((long) (((long) 53)) << 2) | 1));
		      BgL_auxz00_4272 = (int) (((long) 0));
		      ((&(((obj_t) (BgL_v1049z00_1021))->vector_t.obj0))
		       [BgL_auxz00_4272] =
		       BgL_auxz00_4274,
		       ((obj_t) (obj_t) ((long) (((long) (3) << 2) | 2))));
		    }
		    return BgL_v1049z00_1021;
		  }
		}
	      }
	    }
	  }
	} else {
	  int BgL_testz00_4277;
	  {
	    obj_t BgL_auxz00_4278;
	    BgL_auxz00_4278 =
	      ((((obj_t) ((long) BgL_formalsz00_39 - 3))->pair_t).car);
	    BgL_testz00_4277 =
	      ((((long) BgL_auxz00_4278) & ((1 << 2) - 1)) == 3);
	  }
	  if (BgL_testz00_4277) {
	    goto BgL_tagzd21971zd2_948;
	  } else {
	    if ((BgL_namedzf3zf3_42 !=
		 ((obj_t) (obj_t) ((long) (((long) (1) << 2) | 2))))) {
	      obj_t BgL_v1046z00_1014;
	      {
		int BgL_auxz00_4283;
		BgL_auxz00_4283 = (int) (((long) 3));
		BgL_v1046z00_1014 = create_vector (BgL_auxz00_4283);
	      }
	      {
		obj_t BgL_arg1600z00_1016;
		BgL_arg1600z00_1016 =
		  make_pair (BgL_wherez00_41, BgL_bodyz00_40);
		{
		  int BgL_auxz00_4287;
		  BgL_auxz00_4287 = (int) (((long) 2));
		  ((&(((obj_t) (BgL_v1046z00_1014))->vector_t.obj0))
		   [BgL_auxz00_4287] =
		   BgL_arg1600z00_1016,
		   ((obj_t) (obj_t) ((long) (((long) (3) << 2) | 2))));
		}
	      }
	      {
		int BgL_auxz00_4290;
		BgL_auxz00_4290 = (int) (((long) 1));
		((&(((obj_t) (BgL_v1046z00_1014))->vector_t.obj0))
		 [BgL_auxz00_4290] =
		 BgL_locz00_43,
		 ((obj_t) (obj_t) ((long) (((long) (3) << 2) | 2))));
	      }
	      {
		obj_t BgL_auxz00_4295;
		int BgL_auxz00_4293;
		BgL_auxz00_4295 =
		  (obj_t) ((long) (((long) (((long) 48)) << 2) | 1));
		BgL_auxz00_4293 = (int) (((long) 0));
		((&(((obj_t) (BgL_v1046z00_1014))->vector_t.obj0))
		 [BgL_auxz00_4293] =
		 BgL_auxz00_4295,
		 ((obj_t) (obj_t) ((long) (((long) (3) << 2) | 2))));
	      }
	      return BgL_v1046z00_1014;
	    } else {
	      obj_t BgL_v1047z00_1017;
	      {
		int BgL_auxz00_4298;
		BgL_auxz00_4298 = (int) (((long) 3));
		BgL_v1047z00_1017 = create_vector (BgL_auxz00_4298);
	      }
	      {
		int BgL_auxz00_4301;
		BgL_auxz00_4301 = (int) (((long) 2));
		((&(((obj_t) (BgL_v1047z00_1017))->vector_t.obj0))
		 [BgL_auxz00_4301] =
		 BgL_bodyz00_40,
		 ((obj_t) (obj_t) ((long) (((long) (3) << 2) | 2))));
	      }
	      {
		int BgL_auxz00_4304;
		BgL_auxz00_4304 = (int) (((long) 1));
		((&(((obj_t) (BgL_v1047z00_1017))->vector_t.obj0))
		 [BgL_auxz00_4304] =
		 BgL_locz00_43,
		 ((obj_t) (obj_t) ((long) (((long) (3) << 2) | 2))));
	      }
	      {
		obj_t BgL_auxz00_4309;
		int BgL_auxz00_4307;
		BgL_auxz00_4309 =
		  (obj_t) ((long) (((long) (((long) 52)) << 2) | 1));
		BgL_auxz00_4307 = (int) (((long) 0));
		((&(((obj_t) (BgL_v1047z00_1017))->vector_t.obj0))
		 [BgL_auxz00_4307] =
		 BgL_auxz00_4309,
		 ((obj_t) (obj_t) ((long) (((long) (3) << 2) | 2))));
	      }
	      return BgL_v1047z00_1017;
	    }
	  }
	}
      }
    } else {
      if ((BgL_namedzf3zf3_42 !=
	   ((obj_t) (obj_t) ((long) (((long) (1) << 2) | 2))))) {
	obj_t BgL_v1044z00_1010;
	{
	  int BgL_auxz00_4314;
	  BgL_auxz00_4314 = (int) (((long) 3));
	  BgL_v1044z00_1010 = create_vector (BgL_auxz00_4314);
	}
	{
	  obj_t BgL_arg1598z00_1012;
	  BgL_arg1598z00_1012 = make_pair (BgL_wherez00_41, BgL_bodyz00_40);
	  {
	    int BgL_auxz00_4318;
	    BgL_auxz00_4318 = (int) (((long) 2));
	    ((&(((obj_t) (BgL_v1044z00_1010))->vector_t.obj0))
	     [BgL_auxz00_4318] =
	     BgL_arg1598z00_1012,
	     ((obj_t) (obj_t) ((long) (((long) (3) << 2) | 2))));
	  }
	}
	{
	  int BgL_auxz00_4321;
	  BgL_auxz00_4321 = (int) (((long) 1));
	  ((&(((obj_t) (BgL_v1044z00_1010))->vector_t.obj0))[BgL_auxz00_4321]
	   =
	   BgL_locz00_43, ((obj_t) (obj_t) ((long) (((long) (3) << 2) | 2))));
	}
	{
	  obj_t BgL_auxz00_4326;
	  int BgL_auxz00_4324;
	  BgL_auxz00_4326 =
	    (obj_t) ((long) (((long) (((long) 47)) << 2) | 1));
	  BgL_auxz00_4324 = (int) (((long) 0));
	  ((&(((obj_t) (BgL_v1044z00_1010))->vector_t.obj0))[BgL_auxz00_4324]
	   =
	   BgL_auxz00_4326,
	   ((obj_t) (obj_t) ((long) (((long) (3) << 2) | 2))));
	}
	return BgL_v1044z00_1010;
      } else {
	obj_t BgL_v1045z00_1013;
	{
	  int BgL_auxz00_4329;
	  BgL_auxz00_4329 = (int) (((long) 3));
	  BgL_v1045z00_1013 = create_vector (BgL_auxz00_4329);
	}
	{
	  int BgL_auxz00_4332;
	  BgL_auxz00_4332 = (int) (((long) 2));
	  ((&(((obj_t) (BgL_v1045z00_1013))->vector_t.obj0))[BgL_auxz00_4332]
	   =
	   BgL_bodyz00_40,
	   ((obj_t) (obj_t) ((long) (((long) (3) << 2) | 2))));
	}
	{
	  int BgL_auxz00_4335;
	  BgL_auxz00_4335 = (int) (((long) 1));
	  ((&(((obj_t) (BgL_v1045z00_1013))->vector_t.obj0))[BgL_auxz00_4335]
	   =
	   BgL_locz00_43, ((obj_t) (obj_t) ((long) (((long) (3) << 2) | 2))));
	}
	{
	  obj_t BgL_auxz00_4340;
	  int BgL_auxz00_4338;
	  BgL_auxz00_4340 =
	    (obj_t) ((long) (((long) (((long) 51)) << 2) | 1));
	  BgL_auxz00_4338 = (int) (((long) 0));
	  ((&(((obj_t) (BgL_v1045z00_1013))->vector_t.obj0))[BgL_auxz00_4338]
	   =
	   BgL_auxz00_4340,
	   ((obj_t) (obj_t) ((long) (((long) (3) << 2) | 2))));
	}
	return BgL_v1045z00_1013;
      }
    }
  }
}

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-unknown-linux-gnu" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f80;
// DEFAULT-NEXT:         storage bool [size=1, align=1];
// DEFAULT-NEXT:         storage i8, u8 [size=1, align=1];
// DEFAULT-NEXT:         storage i16, u16 [size=2, align=2];
// DEFAULT-NEXT:         storage i32, u32 [size=4, align=4];
// DEFAULT-NEXT:         storage i64, u64 [size=8, align=8];
// DEFAULT-NEXT:         storage i128, u128 [size=16, align=16];
// DEFAULT-NEXT:         storage bf16 [size=2, align=2];
// DEFAULT-NEXT:         storage f16 [size=2, align=2];
// DEFAULT-NEXT:         storage f32 [size=4, align=4];
// DEFAULT-NEXT:         storage f64 [size=8, align=8];
// DEFAULT-NEXT:         storage f80 [size=16, align=16];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     type @type0 scmobj = union {
// DEFAULT-NEXT:         field0 pair_t: @type1;
// DEFAULT-NEXT:         field1 vector_t: @type2;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type1 pair = struct {
// DEFAULT-NEXT:         field0 car: ptr<@type0>;
// DEFAULT-NEXT:         field1 cdr: ptr<@type0>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type2 vector = struct {
// DEFAULT-NEXT:         field0 header: i64;
// DEFAULT-NEXT:         field1 length: i32;
// DEFAULT-NEXT:         field2 obj0: ptr<@type0>;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// DEFAULT-NEXT:     type @type3 obj_t = ptr<@type0>;
// DEFAULT-NEXT:     fn %4 @create_vector(%132 <unnamed>: i32) -> ptr<@type0> [linkage=external];
// DEFAULT-NEXT:     fn %5 @make_pair(%133 <unnamed>: ptr<@type0>, %134 <unnamed>: ptr<@type0>) -> ptr<@type0> [linkage=external];
// DEFAULT-NEXT:     fn %6 @bgl_list_length(%135 <unnamed>: ptr<@type0>) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %7 @BGl_equalzf3zf3zz__r4_equivalence_6_2z00(%136 <unnamed>: ptr<@type0>, %137 <unnamed>: ptr<@type0>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %8 @BGl_evcompilezd2lambdazd2zz__evcompilez00(%11 BgL_formalsz00_39: ptr<@type0>, %12 BgL_bodyz00_40: ptr<@type0>, %13 BgL_wherez00_41: ptr<@type0>, %14 BgL_namedzf3zf3_42: ptr<@type0>, %15 BgL_locz00_43: ptr<@type0>) -> ptr<@type0> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type0>, ptr<@type0>) -> i32>(%7, read<ptr<@type0>>(%11), int_to_ptr<ptr<@type0>, reason=explicit>(or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i64, reason=explicit>(const<i32>(0)), const<i32>(2)), widen<i64, reason=usual_arith>(const<i32>(2))))), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 label %9 BgL_tagzd21966zd2_943:
// DEFAULT-NEXT:                     if ne<ptr<@type0>>(read<ptr<@type0>>(%14), int_to_ptr<ptr<@type0>, reason=explicit>(or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i64, reason=explicit>(const<i32>(1)), const<i32>(2)), widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             let %16 BgL_v1042z00_998: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %17 BgL_auxz00_4066: i32 [storage=automatic];
// DEFAULT-NEXT:                                 write<i32>(%17, truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(3))));
// DEFAULT-NEXT:                                 write<ptr<@type0>>(%16, call<ptr<@type0>, signature=fn(i32) -> ptr<@type0>>(%4, read<i32>(%17)));
// DEFAULT-NEXT:                                 call<ptr<@type0>, signature=fn(i32) -> ptr<@type0>>(%4, read<i32>(%17));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %18 BgL_arg1586z00_1000: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:                                 write<ptr<@type0>>(%18, call<ptr<@type0>, signature=fn(ptr<@type0>, ptr<@type0>) -> ptr<@type0>>(%5, read<ptr<@type0>>(%13), read<ptr<@type0>>(%12)));
// DEFAULT-NEXT:                                 call<ptr<@type0>, signature=fn(ptr<@type0>, ptr<@type0>) -> ptr<@type0>>(%5, read<ptr<@type0>>(%13), read<ptr<@type0>>(%12));
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     let %19 BgL_auxz00_4070: i32 [storage=automatic];
// DEFAULT-NEXT:                                     write<i32>(%19, truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))));
// DEFAULT-NEXT:                                     write<ptr<@type0>>(deref(ptr_offset<ptr<ptr<@type0>>, subtract=false, element=ptr<@type0>, overflow=ub>(addr_of<ptr<ptr<@type0>>>(field2(field1(deref(read<ptr<@type0>>(%16))))), read<i32>(%19))), read<ptr<@type0>>(%18));
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %20 BgL_auxz00_4073: i32 [storage=automatic];
// DEFAULT-NEXT:                                 write<i32>(%20, truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))));
// DEFAULT-NEXT:                                 write<ptr<@type0>>(deref(ptr_offset<ptr<ptr<@type0>>, subtract=false, element=ptr<@type0>, overflow=ub>(addr_of<ptr<ptr<@type0>>>(field2(field1(deref(read<ptr<@type0>>(%16))))), read<i32>(%20))), read<ptr<@type0>>(%15));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %21 BgL_auxz00_4078: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:                                 let %22 BgL_auxz00_4076: i32 [storage=automatic];
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     let %23 BgL_auxz00_4079: i64 [storage=automatic];
// DEFAULT-NEXT:                                     {
// DEFAULT-NEXT:                                         let %24 BgL_auxz00_4080: i64 [storage=automatic];
// DEFAULT-NEXT:                                         write<i64>(%24, call<i64, signature=fn(ptr<@type0>) -> i64>(%6, read<ptr<@type0>>(%11)));
// DEFAULT-NEXT:                                         call<i64, signature=fn(ptr<@type0>) -> i64>(%6, read<ptr<@type0>>(%11));
// DEFAULT-NEXT:                                         write<i64>(%23, add<i64, overflow=ub>(read<i64>(%24), widen<i64, reason=explicit>(const<i32>(37))));
// DEFAULT-NEXT:                                     }
// DEFAULT-NEXT:                                     write<ptr<@type0>>(%21, int_to_ptr<ptr<@type0>, reason=explicit>(or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i64>(%23), const<i32>(2)), widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 write<i32>(%22, truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))));
// DEFAULT-NEXT:                                 write<ptr<@type0>>(deref(ptr_offset<ptr<ptr<@type0>>, subtract=false, element=ptr<@type0>, overflow=ub>(addr_of<ptr<ptr<@type0>>>(field2(field1(deref(read<ptr<@type0>>(%16))))), read<i32>(%22))), read<ptr<@type0>>(%21));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                             return read<ptr<@type0>>(%16);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             let %25 BgL_v1043z00_1005: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %26 BgL_auxz00_4085: i32 [storage=automatic];
// DEFAULT-NEXT:                                 write<i32>(%26, truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(3))));
// DEFAULT-NEXT:                                 write<ptr<@type0>>(%25, call<ptr<@type0>, signature=fn(i32) -> ptr<@type0>>(%4, read<i32>(%26)));
// DEFAULT-NEXT:                                 call<ptr<@type0>, signature=fn(i32) -> ptr<@type0>>(%4, read<i32>(%26));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %27 BgL_auxz00_4088: i32 [storage=automatic];
// DEFAULT-NEXT:                                 write<i32>(%27, truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))));
// DEFAULT-NEXT:                                 write<ptr<@type0>>(deref(ptr_offset<ptr<ptr<@type0>>, subtract=false, element=ptr<@type0>, overflow=ub>(addr_of<ptr<ptr<@type0>>>(field2(field1(deref(read<ptr<@type0>>(%25))))), read<i32>(%27))), read<ptr<@type0>>(%12));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %28 BgL_auxz00_4091: i32 [storage=automatic];
// DEFAULT-NEXT:                                 write<i32>(%28, truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))));
// DEFAULT-NEXT:                                 write<ptr<@type0>>(deref(ptr_offset<ptr<ptr<@type0>>, subtract=false, element=ptr<@type0>, overflow=ub>(addr_of<ptr<ptr<@type0>>>(field2(field1(deref(read<ptr<@type0>>(%25))))), read<i32>(%28))), read<ptr<@type0>>(%15));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %29 BgL_auxz00_4096: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:                                 let %30 BgL_auxz00_4094: i32 [storage=automatic];
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     let %31 BgL_auxz00_4097: i64 [storage=automatic];
// DEFAULT-NEXT:                                     {
// DEFAULT-NEXT:                                         let %32 BgL_auxz00_4098: i64 [storage=automatic];
// DEFAULT-NEXT:                                         write<i64>(%32, call<i64, signature=fn(ptr<@type0>) -> i64>(%6, read<ptr<@type0>>(%11)));
// DEFAULT-NEXT:                                         call<i64, signature=fn(ptr<@type0>) -> i64>(%6, read<ptr<@type0>>(%11));
// DEFAULT-NEXT:                                         write<i64>(%31, add<i64, overflow=ub>(read<i64>(%32), widen<i64, reason=explicit>(const<i32>(42))));
// DEFAULT-NEXT:                                     }
// DEFAULT-NEXT:                                     write<ptr<@type0>>(%29, int_to_ptr<ptr<@type0>, reason=explicit>(or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i64>(%31), const<i32>(2)), widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 write<i32>(%30, truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))));
// DEFAULT-NEXT:                                 write<ptr<@type0>>(deref(ptr_offset<ptr<ptr<@type0>>, subtract=false, element=ptr<@type0>, overflow=ub>(addr_of<ptr<ptr<@type0>>>(field2(field1(deref(read<ptr<@type0>>(%25))))), read<i32>(%30))), read<ptr<@type0>>(%29));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                             return read<ptr<@type0>>(%25);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if eq<i64>(and<i64>(ptr_to_int<i64, reason=explicit>(read<ptr<@type0>>(%11)), widen<i64, reason=usual_arith>(sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(2)), const<i32>(1)))), widen<i64, reason=usual_arith>(const<i32>(3)))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(call<i32, signature=fn(ptr<@type0>, ptr<@type0>) -> i32>(%7, read<ptr<@type0>>(field1(field0(deref(int_to_ptr<ptr<@type0>, reason=explicit>(sub<i64, overflow=ub>(ptr_to_int<i64, reason=explicit>(read<ptr<@type0>>(%11)), widen<i64, reason=usual_arith>(const<i32>(3)))))))), int_to_ptr<ptr<@type0>, reason=explicit>(or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i64, reason=explicit>(const<i32>(0)), const<i32>(2)), widen<i64, reason=usual_arith>(const<i32>(2))))), const<i32>(0))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 goto %9;
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %33 BgL_cdrzd21979zd2_953: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:                                 write<ptr<@type0>>(%33, read<ptr<@type0>>(field1(field0(deref(int_to_ptr<ptr<@type0>, reason=explicit>(sub<i64, overflow=ub>(ptr_to_int<i64, reason=explicit>(read<ptr<@type0>>(%11)), widen<i64, reason=usual_arith>(const<i32>(3)))))))));
// DEFAULT-NEXT:                                 if eq<i64>(and<i64>(ptr_to_int<i64, reason=explicit>(read<ptr<@type0>>(%33)), widen<i64, reason=usual_arith>(sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(2)), const<i32>(1)))), widen<i64, reason=usual_arith>(const<i32>(3)))
// DEFAULT-NEXT:                                     {
// DEFAULT-NEXT:                                         if ne<i32>(call<i32, signature=fn(ptr<@type0>, ptr<@type0>) -> i32>(%7, read<ptr<@type0>>(field1(field0(deref(int_to_ptr<ptr<@type0>, reason=explicit>(sub<i64, overflow=ub>(ptr_to_int<i64, reason=explicit>(read<ptr<@type0>>(%33)), widen<i64, reason=usual_arith>(const<i32>(3)))))))), int_to_ptr<ptr<@type0>, reason=explicit>(or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i64, reason=explicit>(const<i32>(0)), const<i32>(2)), widen<i64, reason=usual_arith>(const<i32>(2))))), const<i32>(0))
// DEFAULT-NEXT:                                             {
// DEFAULT-NEXT:                                                 goto %9;
// DEFAULT-NEXT:                                             }
// DEFAULT-NEXT:                                         else
// DEFAULT-NEXT:                                             {
// DEFAULT-NEXT:                                                 let %34 BgL_cdrzd21986zd2_956: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:                                                 write<ptr<@type0>>(%34, read<ptr<@type0>>(field1(field0(deref(int_to_ptr<ptr<@type0>, reason=explicit>(sub<i64, overflow=ub>(ptr_to_int<i64, reason=explicit>(read<ptr<@type0>>(%33)), widen<i64, reason=usual_arith>(const<i32>(3)))))))));
// DEFAULT-NEXT:                                                 if eq<i64>(and<i64>(ptr_to_int<i64, reason=explicit>(read<ptr<@type0>>(%34)), widen<i64, reason=usual_arith>(sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(2)), const<i32>(1)))), widen<i64, reason=usual_arith>(const<i32>(3)))
// DEFAULT-NEXT:                                                     {
// DEFAULT-NEXT:                                                         if ne<i32>(call<i32, signature=fn(ptr<@type0>, ptr<@type0>) -> i32>(%7, read<ptr<@type0>>(field1(field0(deref(int_to_ptr<ptr<@type0>, reason=explicit>(sub<i64, overflow=ub>(ptr_to_int<i64, reason=explicit>(read<ptr<@type0>>(%34)), widen<i64, reason=usual_arith>(const<i32>(3)))))))), int_to_ptr<ptr<@type0>, reason=explicit>(or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i64, reason=explicit>(const<i32>(0)), const<i32>(2)), widen<i64, reason=usual_arith>(const<i32>(2))))), const<i32>(0))
// DEFAULT-NEXT:                                                             {
// DEFAULT-NEXT:                                                                 goto %9;
// DEFAULT-NEXT:                                                             }
// DEFAULT-NEXT:                                                         else
// DEFAULT-NEXT:                                                             {
// DEFAULT-NEXT:                                                                 let %35 BgL_cdrzd21994zd2_959: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:                                                                 {
// DEFAULT-NEXT:                                                                     let %36 BgL_auxz00_4120: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:                                                                     write<ptr<@type0>>(%36, read<ptr<@type0>>(field1(field0(deref(int_to_ptr<ptr<@type0>, reason=explicit>(sub<i64, overflow=ub>(ptr_to_int<i64, reason=explicit>(read<ptr<@type0>>(%33)), widen<i64, reason=usual_arith>(const<i32>(3)))))))));
// DEFAULT-NEXT:                                                                     write<ptr<@type0>>(%35, read<ptr<@type0>>(field1(field0(deref(int_to_ptr<ptr<@type0>, reason=explicit>(sub<i64, overflow=ub>(ptr_to_int<i64, reason=explicit>(read<ptr<@type0>>(%36)), widen<i64, reason=usual_arith>(const<i32>(3)))))))));
// DEFAULT-NEXT:                                                                 }
// DEFAULT-NEXT:                                                                 if eq<i64>(and<i64>(ptr_to_int<i64, reason=explicit>(read<ptr<@type0>>(%35)), widen<i64, reason=usual_arith>(sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(2)), const<i32>(1)))), widen<i64, reason=usual_arith>(const<i32>(3)))
// DEFAULT-NEXT:                                                                     {
// DEFAULT-NEXT:                                                                         if ne<i32>(call<i32, signature=fn(ptr<@type0>, ptr<@type0>) -> i32>(%7, read<ptr<@type0>>(field1(field0(deref(int_to_ptr<ptr<@type0>, reason=explicit>(sub<i64, overflow=ub>(ptr_to_int<i64, reason=explicit>(read<ptr<@type0>>(%35)), widen<i64, reason=usual_arith>(const<i32>(3)))))))), int_to_ptr<ptr<@type0>, reason=explicit>(or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i64, reason=explicit>(const<i32>(0)), const<i32>(2)), widen<i64, reason=usual_arith>(const<i32>(2))))), const<i32>(0))
// DEFAULT-NEXT:                                                                             {
// DEFAULT-NEXT:                                                                                 goto %9;
// DEFAULT-NEXT:                                                                             }
// DEFAULT-NEXT:                                                                         else
// DEFAULT-NEXT:                                                                             {
// DEFAULT-NEXT:                                                                                 let %37 BgL_testz00_4128: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                                 {
// DEFAULT-NEXT:                                                                                     let %38 BgL_auxz00_4129: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:                                                                                     write<ptr<@type0>>(%38, read<ptr<@type0>>(field0(field0(deref(int_to_ptr<ptr<@type0>, reason=explicit>(sub<i64, overflow=ub>(ptr_to_int<i64, reason=explicit>(read<ptr<@type0>>(%11)), widen<i64, reason=usual_arith>(const<i32>(3)))))))));
// DEFAULT-NEXT:                                                                                     write<i32>(%37, from_bool<i32, reason=assign>(eq<i64>(and<i64>(ptr_to_int<i64, reason=explicit>(read<ptr<@type0>>(%38)), widen<i64, reason=usual_arith>(sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(2)), const<i32>(1)))), widen<i64, reason=usual_arith>(const<i32>(3)))));
// DEFAULT-NEXT:                                                                                 }
// DEFAULT-NEXT:                                                                                 if ne<i32>(read<i32>(%37), const<i32>(0))
// DEFAULT-NEXT:                                                                                     {
// DEFAULT-NEXT:                                                                                         label %10 BgL_tagzd21971zd2_948:
// DEFAULT-NEXT:                                                                                             if ne<ptr<@type0>>(read<ptr<@type0>>(%14), int_to_ptr<ptr<@type0>, reason=explicit>(or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i64, reason=explicit>(const<i32>(1)), const<i32>(2)), widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:                                                                                                 {
// DEFAULT-NEXT:                                                                                                     let %39 BgL_v1052z00_1026: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:                                                                                                     {
// DEFAULT-NEXT:                                                                                                         let %40 BgL_auxz00_4134: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                                                         write<i32>(%40, truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(3))));
// DEFAULT-NEXT:                                                                                                         write<ptr<@type0>>(%39, call<ptr<@type0>, signature=fn(i32) -> ptr<@type0>>(%4, read<i32>(%40)));
// DEFAULT-NEXT:                                                                                                         call<ptr<@type0>, signature=fn(i32) -> ptr<@type0>>(%4, read<i32>(%40));
// DEFAULT-NEXT:                                                                                                     }
// DEFAULT-NEXT:                                                                                                     {
// DEFAULT-NEXT:                                                                                                         let %41 BgL_arg1606z00_1028: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:                                                                                                         {
// DEFAULT-NEXT:                                                                                                             let %42 BgL_v1053z00_1029: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:                                                                                                             {
// DEFAULT-NEXT:                                                                                                                 let %43 BgL_auxz00_4137: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                                                                 write<i32>(%43, truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(3))));
// DEFAULT-NEXT:                                                                                                                 write<ptr<@type0>>(%42, call<ptr<@type0>, signature=fn(i32) -> ptr<@type0>>(%4, read<i32>(%43)));
// DEFAULT-NEXT:                                                                                                                 call<ptr<@type0>, signature=fn(i32) -> ptr<@type0>>(%4, read<i32>(%43));
// DEFAULT-NEXT:                                                                                                             }
// DEFAULT-NEXT:                                                                                                             {
// DEFAULT-NEXT:                                                                                                                 let %44 BgL_auxz00_4140: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                                                                 write<i32>(%44, truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))));
// DEFAULT-NEXT:                                                                                                                 write<ptr<@type0>>(deref(ptr_offset<ptr<ptr<@type0>>, subtract=false, element=ptr<@type0>, overflow=ub>(addr_of<ptr<ptr<@type0>>>(field2(field1(deref(read<ptr<@type0>>(%42))))), read<i32>(%44))), read<ptr<@type0>>(%11));
// DEFAULT-NEXT:                                                                                                             }
// DEFAULT-NEXT:                                                                                                             {
// DEFAULT-NEXT:                                                                                                                 let %45 BgL_auxz00_4143: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                                                                 write<i32>(%45, truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))));
// DEFAULT-NEXT:                                                                                                                 write<ptr<@type0>>(deref(ptr_offset<ptr<ptr<@type0>>, subtract=false, element=ptr<@type0>, overflow=ub>(addr_of<ptr<ptr<@type0>>>(field2(field1(deref(read<ptr<@type0>>(%42))))), read<i32>(%45))), read<ptr<@type0>>(%12));
// DEFAULT-NEXT:                                                                                                             }
// DEFAULT-NEXT:                                                                                                             {
// DEFAULT-NEXT:                                                                                                                 let %46 BgL_auxz00_4146: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                                                                 write<i32>(%46, truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))));
// DEFAULT-NEXT:                                                                                                                 write<ptr<@type0>>(deref(ptr_offset<ptr<ptr<@type0>>, subtract=false, element=ptr<@type0>, overflow=ub>(addr_of<ptr<ptr<@type0>>>(field2(field1(deref(read<ptr<@type0>>(%42))))), read<i32>(%46))), read<ptr<@type0>>(%13));
// DEFAULT-NEXT:                                                                                                             }
// DEFAULT-NEXT:                                                                                                             write<ptr<@type0>>(%41, read<ptr<@type0>>(%42));
// DEFAULT-NEXT:                                                                                                         }
// DEFAULT-NEXT:                                                                                                         {
// DEFAULT-NEXT:                                                                                                             let %47 BgL_auxz00_4149: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                                                             write<i32>(%47, truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))));
// DEFAULT-NEXT:                                                                                                             write<ptr<@type0>>(deref(ptr_offset<ptr<ptr<@type0>>, subtract=false, element=ptr<@type0>, overflow=ub>(addr_of<ptr<ptr<@type0>>>(field2(field1(deref(read<ptr<@type0>>(%39))))), read<i32>(%47))), read<ptr<@type0>>(%41));
// DEFAULT-NEXT:                                                                                                         }
// DEFAULT-NEXT:                                                                                                     }
// DEFAULT-NEXT:                                                                                                     {
// DEFAULT-NEXT:                                                                                                         let %48 BgL_auxz00_4152: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                                                         write<i32>(%48, truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))));
// DEFAULT-NEXT:                                                                                                         write<ptr<@type0>>(deref(ptr_offset<ptr<ptr<@type0>>, subtract=false, element=ptr<@type0>, overflow=ub>(addr_of<ptr<ptr<@type0>>>(field2(field1(deref(read<ptr<@type0>>(%39))))), read<i32>(%48))), read<ptr<@type0>>(%15));
// DEFAULT-NEXT:                                                                                                     }
// DEFAULT-NEXT:                                                                                                     {
// DEFAULT-NEXT:                                                                                                         let %49 BgL_auxz00_4157: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:                                                                                                         let %50 BgL_auxz00_4155: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                                                         write<ptr<@type0>>(%49, int_to_ptr<ptr<@type0>, reason=explicit>(or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i64, reason=explicit>(const<i32>(55)), const<i32>(2)), widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                                                                                         write<i32>(%50, truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))));
// DEFAULT-NEXT:                                                                                                         write<ptr<@type0>>(deref(ptr_offset<ptr<ptr<@type0>>, subtract=false, element=ptr<@type0>, overflow=ub>(addr_of<ptr<ptr<@type0>>>(field2(field1(deref(read<ptr<@type0>>(%39))))), read<i32>(%50))), read<ptr<@type0>>(%49));
// DEFAULT-NEXT:                                                                                                     }
// DEFAULT-NEXT:                                                                                                     return read<ptr<@type0>>(%39);
// DEFAULT-NEXT:                                                                                                 }
// DEFAULT-NEXT:                                                                                             else
// DEFAULT-NEXT:                                                                                                 {
// DEFAULT-NEXT:                                                                                                     let %51 BgL_v1054z00_1030: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:                                                                                                     {
// DEFAULT-NEXT:                                                                                                         let %52 BgL_auxz00_4160: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                                                         write<i32>(%52, truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(3))));
// DEFAULT-NEXT:                                                                                                         write<ptr<@type0>>(%51, call<ptr<@type0>, signature=fn(i32) -> ptr<@type0>>(%4, read<i32>(%52)));
// DEFAULT-NEXT:                                                                                                         call<ptr<@type0>, signature=fn(i32) -> ptr<@type0>>(%4, read<i32>(%52));
// DEFAULT-NEXT:                                                                                                     }
// DEFAULT-NEXT:                                                                                                     {
// DEFAULT-NEXT:                                                                                                         let %53 BgL_arg1608z00_1032: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:                                                                                                         write<ptr<@type0>>(%53, call<ptr<@type0>, signature=fn(ptr<@type0>, ptr<@type0>) -> ptr<@type0>>(%5, read<ptr<@type0>>(%12), read<ptr<@type0>>(%11)));
// DEFAULT-NEXT:                                                                                                         call<ptr<@type0>, signature=fn(ptr<@type0>, ptr<@type0>) -> ptr<@type0>>(%5, read<ptr<@type0>>(%12), read<ptr<@type0>>(%11));
// DEFAULT-NEXT:                                                                                                         {
// DEFAULT-NEXT:                                                                                                             let %54 BgL_auxz00_4164: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                                                             write<i32>(%54, truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))));
// DEFAULT-NEXT:                                                                                                             write<ptr<@type0>>(deref(ptr_offset<ptr<ptr<@type0>>, subtract=false, element=ptr<@type0>, overflow=ub>(addr_of<ptr<ptr<@type0>>>(field2(field1(deref(read<ptr<@type0>>(%51))))), read<i32>(%54))), read<ptr<@type0>>(%53));
// DEFAULT-NEXT:                                                                                                         }
// DEFAULT-NEXT:                                                                                                     }
// DEFAULT-NEXT:                                                                                                     {
// DEFAULT-NEXT:                                                                                                         let %55 BgL_auxz00_4167: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                                                         write<i32>(%55, truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))));
// DEFAULT-NEXT:                                                                                                         write<ptr<@type0>>(deref(ptr_offset<ptr<ptr<@type0>>, subtract=false, element=ptr<@type0>, overflow=ub>(addr_of<ptr<ptr<@type0>>>(field2(field1(deref(read<ptr<@type0>>(%51))))), read<i32>(%55))), read<ptr<@type0>>(%15));
// DEFAULT-NEXT:                                                                                                     }
// DEFAULT-NEXT:                                                                                                     {
// DEFAULT-NEXT:                                                                                                         let %56 BgL_auxz00_4172: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:                                                                                                         let %57 BgL_auxz00_4170: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                                                         write<ptr<@type0>>(%56, int_to_ptr<ptr<@type0>, reason=explicit>(or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i64, reason=explicit>(const<i32>(56)), const<i32>(2)), widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                                                                                         write<i32>(%57, truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))));
// DEFAULT-NEXT:                                                                                                         write<ptr<@type0>>(deref(ptr_offset<ptr<ptr<@type0>>, subtract=false, element=ptr<@type0>, overflow=ub>(addr_of<ptr<ptr<@type0>>>(field2(field1(deref(read<ptr<@type0>>(%51))))), read<i32>(%57))), read<ptr<@type0>>(%56));
// DEFAULT-NEXT:                                                                                                     }
// DEFAULT-NEXT:                                                                                                     return read<ptr<@type0>>(%51);
// DEFAULT-NEXT:                                                                                                 }
// DEFAULT-NEXT:                                                                                     }
// DEFAULT-NEXT:                                                                                 else
// DEFAULT-NEXT:                                                                                     {
// DEFAULT-NEXT:                                                                                         let %58 BgL_testz00_4175: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                                         {
// DEFAULT-NEXT:                                                                                             let %59 BgL_auxz00_4176: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:                                                                                             {
// DEFAULT-NEXT:                                                                                                 let %60 BgL_auxz00_4177: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:                                                                                                 write<ptr<@type0>>(%60, read<ptr<@type0>>(field1(field0(deref(int_to_ptr<ptr<@type0>, reason=explicit>(sub<i64, overflow=ub>(ptr_to_int<i64, reason=explicit>(read<ptr<@type0>>(%11)), widen<i64, reason=usual_arith>(const<i32>(3)))))))));
// DEFAULT-NEXT:                                                                                                 write<ptr<@type0>>(%59, read<ptr<@type0>>(field0(field0(deref(int_to_ptr<ptr<@type0>, reason=explicit>(sub<i64, overflow=ub>(ptr_to_int<i64, reason=explicit>(read<ptr<@type0>>(%60)), widen<i64, reason=usual_arith>(const<i32>(3)))))))));
// DEFAULT-NEXT:                                                                                             }
// DEFAULT-NEXT:                                                                                             write<i32>(%58, from_bool<i32, reason=assign>(eq<i64>(and<i64>(ptr_to_int<i64, reason=explicit>(read<ptr<@type0>>(%59)), widen<i64, reason=usual_arith>(sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(2)), const<i32>(1)))), widen<i64, reason=usual_arith>(const<i32>(3)))));
// DEFAULT-NEXT:                                                                                         }
// DEFAULT-NEXT:                                                                                         if ne<i32>(read<i32>(%58), const<i32>(0))
// DEFAULT-NEXT:                                                                                             {
// DEFAULT-NEXT:                                                                                                 goto %10;
// DEFAULT-NEXT:                                                                                             }
// DEFAULT-NEXT:                                                                                         else
// DEFAULT-NEXT:                                                                                             {
// DEFAULT-NEXT:                                                                                                 let %61 BgL_testz00_4181: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                                                 {
// DEFAULT-NEXT:                                                                                                     let %62 BgL_auxz00_4182: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:                                                                                                     {
// DEFAULT-NEXT:                                                                                                         let %63 BgL_auxz00_4183: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:                                                                                                         {
// DEFAULT-NEXT:                                                                                                             let %64 BgL_auxz00_4184: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:                                                                                                             write<ptr<@type0>>(%64, read<ptr<@type0>>(field1(field0(deref(int_to_ptr<ptr<@type0>, reason=explicit>(sub<i64, overflow=ub>(ptr_to_int<i64, reason=explicit>(read<ptr<@type0>>(%11)), widen<i64, reason=usual_arith>(const<i32>(3)))))))));
// DEFAULT-NEXT:                                                                                                             write<ptr<@type0>>(%63, read<ptr<@type0>>(field1(field0(deref(int_to_ptr<ptr<@type0>, reason=explicit>(sub<i64, overflow=ub>(ptr_to_int<i64, reason=explicit>(read<ptr<@type0>>(%64)), widen<i64, reason=usual_arith>(const<i32>(3)))))))));
// DEFAULT-NEXT:                                                                                                         }
// DEFAULT-NEXT:                                                                                                         write<ptr<@type0>>(%62, read<ptr<@type0>>(field0(field0(deref(int_to_ptr<ptr<@type0>, reason=explicit>(sub<i64, overflow=ub>(ptr_to_int<i64, reason=explicit>(read<ptr<@type0>>(%63)), widen<i64, reason=usual_arith>(const<i32>(3)))))))));
// DEFAULT-NEXT:                                                                                                     }
// DEFAULT-NEXT:                                                                                                     write<i32>(%61, from_bool<i32, reason=assign>(eq<i64>(and<i64>(ptr_to_int<i64, reason=explicit>(read<ptr<@type0>>(%62)), widen<i64, reason=usual_arith>(sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(2)), const<i32>(1)))), widen<i64, reason=usual_arith>(const<i32>(3)))));
// DEFAULT-NEXT:                                                                                                 }
// DEFAULT-NEXT:                                                                                                 if ne<i32>(read<i32>(%61), const<i32>(0))
// DEFAULT-NEXT:                                                                                                     {
// DEFAULT-NEXT:                                                                                                         goto %10;
// DEFAULT-NEXT:                                                                                                     }
// DEFAULT-NEXT:                                                                                                 else
// DEFAULT-NEXT:                                                                                                     {
// DEFAULT-NEXT:                                                                                                         goto %10;
// DEFAULT-NEXT:                                                                                                     }
// DEFAULT-NEXT:                                                                                             }
// DEFAULT-NEXT:                                                                                     }
// DEFAULT-NEXT:                                                                             }
// DEFAULT-NEXT:                                                                     }
// DEFAULT-NEXT:                                                                 else
// DEFAULT-NEXT:                                                                     {
// DEFAULT-NEXT:                                                                         let %65 BgL_testz00_4189: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                         {
// DEFAULT-NEXT:                                                                             let %66 BgL_auxz00_4190: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:                                                                             write<ptr<@type0>>(%66, read<ptr<@type0>>(field0(field0(deref(int_to_ptr<ptr<@type0>, reason=explicit>(sub<i64, overflow=ub>(ptr_to_int<i64, reason=explicit>(read<ptr<@type0>>(%11)), widen<i64, reason=usual_arith>(const<i32>(3)))))))));
// DEFAULT-NEXT:                                                                             write<i32>(%65, from_bool<i32, reason=assign>(eq<i64>(and<i64>(ptr_to_int<i64, reason=explicit>(read<ptr<@type0>>(%66)), widen<i64, reason=usual_arith>(sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(2)), const<i32>(1)))), widen<i64, reason=usual_arith>(const<i32>(3)))));
// DEFAULT-NEXT:                                                                         }
// DEFAULT-NEXT:                                                                         if ne<i32>(read<i32>(%65), const<i32>(0))
// DEFAULT-NEXT:                                                                             {
// DEFAULT-NEXT:                                                                                 goto %10;
// DEFAULT-NEXT:                                                                             }
// DEFAULT-NEXT:                                                                         else
// DEFAULT-NEXT:                                                                             {
// DEFAULT-NEXT:                                                                                 let %67 BgL_testz00_4193: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                                 {
// DEFAULT-NEXT:                                                                                     let %68 BgL_auxz00_4194: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:                                                                                     {
// DEFAULT-NEXT:                                                                                         let %69 BgL_auxz00_4195: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:                                                                                         write<ptr<@type0>>(%69, read<ptr<@type0>>(field1(field0(deref(int_to_ptr<ptr<@type0>, reason=explicit>(sub<i64, overflow=ub>(ptr_to_int<i64, reason=explicit>(read<ptr<@type0>>(%11)), widen<i64, reason=usual_arith>(const<i32>(3)))))))));
// DEFAULT-NEXT:                                                                                         write<ptr<@type0>>(%68, read<ptr<@type0>>(field0(field0(deref(int_to_ptr<ptr<@type0>, reason=explicit>(sub<i64, overflow=ub>(ptr_to_int<i64, reason=explicit>(read<ptr<@type0>>(%69)), widen<i64, reason=usual_arith>(const<i32>(3)))))))));
// DEFAULT-NEXT:                                                                                     }
// DEFAULT-NEXT:                                                                                     write<i32>(%67, from_bool<i32, reason=assign>(eq<i64>(and<i64>(ptr_to_int<i64, reason=explicit>(read<ptr<@type0>>(%68)), widen<i64, reason=usual_arith>(sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(2)), const<i32>(1)))), widen<i64, reason=usual_arith>(const<i32>(3)))));
// DEFAULT-NEXT:                                                                                 }
// DEFAULT-NEXT:                                                                                 if ne<i32>(read<i32>(%67), const<i32>(0))
// DEFAULT-NEXT:                                                                                     {
// DEFAULT-NEXT:                                                                                         goto %10;
// DEFAULT-NEXT:                                                                                     }
// DEFAULT-NEXT:                                                                                 else
// DEFAULT-NEXT:                                                                                     {
// DEFAULT-NEXT:                                                                                         let %70 BgL_testz00_4199: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                                         {
// DEFAULT-NEXT:                                                                                             let %71 BgL_auxz00_4200: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:                                                                                             {
// DEFAULT-NEXT:                                                                                                 let %72 BgL_auxz00_4201: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:                                                                                                 {
// DEFAULT-NEXT:                                                                                                     let %73 BgL_auxz00_4202: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:                                                                                                     write<ptr<@type0>>(%73, read<ptr<@type0>>(field1(field0(deref(int_to_ptr<ptr<@type0>, reason=explicit>(sub<i64, overflow=ub>(ptr_to_int<i64, reason=explicit>(read<ptr<@type0>>(%11)), widen<i64, reason=usual_arith>(const<i32>(3)))))))));
// DEFAULT-NEXT:                                                                                                     write<ptr<@type0>>(%72, read<ptr<@type0>>(field1(field0(deref(int_to_ptr<ptr<@type0>, reason=explicit>(sub<i64, overflow=ub>(ptr_to_int<i64, reason=explicit>(read<ptr<@type0>>(%73)), widen<i64, reason=usual_arith>(const<i32>(3)))))))));
// DEFAULT-NEXT:                                                                                                 }
// DEFAULT-NEXT:                                                                                                 write<ptr<@type0>>(%71, read<ptr<@type0>>(field0(field0(deref(int_to_ptr<ptr<@type0>, reason=explicit>(sub<i64, overflow=ub>(ptr_to_int<i64, reason=explicit>(read<ptr<@type0>>(%72)), widen<i64, reason=usual_arith>(const<i32>(3)))))))));
// DEFAULT-NEXT:                                                                                             }
// DEFAULT-NEXT:                                                                                             write<i32>(%70, from_bool<i32, reason=assign>(eq<i64>(and<i64>(ptr_to_int<i64, reason=explicit>(read<ptr<@type0>>(%71)), widen<i64, reason=usual_arith>(sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(2)), const<i32>(1)))), widen<i64, reason=usual_arith>(const<i32>(3)))));
// DEFAULT-NEXT:                                                                                         }
// DEFAULT-NEXT:                                                                                         if ne<i32>(read<i32>(%70), const<i32>(0))
// DEFAULT-NEXT:                                                                                             {
// DEFAULT-NEXT:                                                                                                 goto %10;
// DEFAULT-NEXT:                                                                                             }
// DEFAULT-NEXT:                                                                                         else
// DEFAULT-NEXT:                                                                                             {
// DEFAULT-NEXT:                                                                                                 if ne<ptr<@type0>>(read<ptr<@type0>>(%14), int_to_ptr<ptr<@type0>, reason=explicit>(or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i64, reason=explicit>(const<i32>(1)), const<i32>(2)), widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:                                                                                                     {
// DEFAULT-NEXT:                                                                                                         let %74 BgL_v1050z00_1022: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:                                                                                                         {
// DEFAULT-NEXT:                                                                                                             let %75 BgL_auxz00_4209: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                                                             write<i32>(%75, truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(3))));
// DEFAULT-NEXT:                                                                                                             write<ptr<@type0>>(%74, call<ptr<@type0>, signature=fn(i32) -> ptr<@type0>>(%4, read<i32>(%75)));
// DEFAULT-NEXT:                                                                                                             call<ptr<@type0>, signature=fn(i32) -> ptr<@type0>>(%4, read<i32>(%75));
// DEFAULT-NEXT:                                                                                                         }
// DEFAULT-NEXT:                                                                                                         {
// DEFAULT-NEXT:                                                                                                             let %76 BgL_arg1604z00_1024: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:                                                                                                             write<ptr<@type0>>(%76, call<ptr<@type0>, signature=fn(ptr<@type0>, ptr<@type0>) -> ptr<@type0>>(%5, read<ptr<@type0>>(%13), read<ptr<@type0>>(%12)));
// DEFAULT-NEXT:                                                                                                             call<ptr<@type0>, signature=fn(ptr<@type0>, ptr<@type0>) -> ptr<@type0>>(%5, read<ptr<@type0>>(%13), read<ptr<@type0>>(%12));
// DEFAULT-NEXT:                                                                                                             {
// DEFAULT-NEXT:                                                                                                                 let %77 BgL_auxz00_4213: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                                                                 write<i32>(%77, truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))));
// DEFAULT-NEXT:                                                                                                                 write<ptr<@type0>>(deref(ptr_offset<ptr<ptr<@type0>>, subtract=false, element=ptr<@type0>, overflow=ub>(addr_of<ptr<ptr<@type0>>>(field2(field1(deref(read<ptr<@type0>>(%74))))), read<i32>(%77))), read<ptr<@type0>>(%76));
// DEFAULT-NEXT:                                                                                                             }
// DEFAULT-NEXT:                                                                                                         }
// DEFAULT-NEXT:                                                                                                         {
// DEFAULT-NEXT:                                                                                                             let %78 BgL_auxz00_4216: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                                                             write<i32>(%78, truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))));
// DEFAULT-NEXT:                                                                                                             write<ptr<@type0>>(deref(ptr_offset<ptr<ptr<@type0>>, subtract=false, element=ptr<@type0>, overflow=ub>(addr_of<ptr<ptr<@type0>>>(field2(field1(deref(read<ptr<@type0>>(%74))))), read<i32>(%78))), read<ptr<@type0>>(%15));
// DEFAULT-NEXT:                                                                                                         }
// DEFAULT-NEXT:                                                                                                         {
// DEFAULT-NEXT:                                                                                                             let %79 BgL_auxz00_4221: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:                                                                                                             let %80 BgL_auxz00_4219: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                                                             write<ptr<@type0>>(%79, int_to_ptr<ptr<@type0>, reason=explicit>(or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i64, reason=explicit>(const<i32>(50)), const<i32>(2)), widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                                                                                             write<i32>(%80, truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))));
// DEFAULT-NEXT:                                                                                                             write<ptr<@type0>>(deref(ptr_offset<ptr<ptr<@type0>>, subtract=false, element=ptr<@type0>, overflow=ub>(addr_of<ptr<ptr<@type0>>>(field2(field1(deref(read<ptr<@type0>>(%74))))), read<i32>(%80))), read<ptr<@type0>>(%79));
// DEFAULT-NEXT:                                                                                                         }
// DEFAULT-NEXT:                                                                                                         return read<ptr<@type0>>(%74);
// DEFAULT-NEXT:                                                                                                     }
// DEFAULT-NEXT:                                                                                                 else
// DEFAULT-NEXT:                                                                                                     {
// DEFAULT-NEXT:                                                                                                         let %81 BgL_v1051z00_1025: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:                                                                                                         {
// DEFAULT-NEXT:                                                                                                             let %82 BgL_auxz00_4224: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                                                             write<i32>(%82, truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(3))));
// DEFAULT-NEXT:                                                                                                             write<ptr<@type0>>(%81, call<ptr<@type0>, signature=fn(i32) -> ptr<@type0>>(%4, read<i32>(%82)));
// DEFAULT-NEXT:                                                                                                             call<ptr<@type0>, signature=fn(i32) -> ptr<@type0>>(%4, read<i32>(%82));
// DEFAULT-NEXT:                                                                                                         }
// DEFAULT-NEXT:                                                                                                         {
// DEFAULT-NEXT:                                                                                                             let %83 BgL_auxz00_4227: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                                                             write<i32>(%83, truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))));
// DEFAULT-NEXT:                                                                                                             write<ptr<@type0>>(deref(ptr_offset<ptr<ptr<@type0>>, subtract=false, element=ptr<@type0>, overflow=ub>(addr_of<ptr<ptr<@type0>>>(field2(field1(deref(read<ptr<@type0>>(%81))))), read<i32>(%83))), read<ptr<@type0>>(%12));
// DEFAULT-NEXT:                                                                                                         }
// DEFAULT-NEXT:                                                                                                         {
// DEFAULT-NEXT:                                                                                                             let %84 BgL_auxz00_4230: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                                                             write<i32>(%84, truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))));
// DEFAULT-NEXT:                                                                                                             write<ptr<@type0>>(deref(ptr_offset<ptr<ptr<@type0>>, subtract=false, element=ptr<@type0>, overflow=ub>(addr_of<ptr<ptr<@type0>>>(field2(field1(deref(read<ptr<@type0>>(%81))))), read<i32>(%84))), read<ptr<@type0>>(%15));
// DEFAULT-NEXT:                                                                                                         }
// DEFAULT-NEXT:                                                                                                         {
// DEFAULT-NEXT:                                                                                                             let %85 BgL_auxz00_4235: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:                                                                                                             let %86 BgL_auxz00_4233: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                                                             write<ptr<@type0>>(%85, int_to_ptr<ptr<@type0>, reason=explicit>(or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i64, reason=explicit>(const<i32>(54)), const<i32>(2)), widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                                                                                             write<i32>(%86, truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))));
// DEFAULT-NEXT:                                                                                                             write<ptr<@type0>>(deref(ptr_offset<ptr<ptr<@type0>>, subtract=false, element=ptr<@type0>, overflow=ub>(addr_of<ptr<ptr<@type0>>>(field2(field1(deref(read<ptr<@type0>>(%81))))), read<i32>(%86))), read<ptr<@type0>>(%85));
// DEFAULT-NEXT:                                                                                                         }
// DEFAULT-NEXT:                                                                                                         return read<ptr<@type0>>(%81);
// DEFAULT-NEXT:                                                                                                     }
// DEFAULT-NEXT:                                                                                             }
// DEFAULT-NEXT:                                                                                     }
// DEFAULT-NEXT:                                                                             }
// DEFAULT-NEXT:                                                                     }
// DEFAULT-NEXT:                                                             }
// DEFAULT-NEXT:                                                     }
// DEFAULT-NEXT:                                                 else
// DEFAULT-NEXT:                                                     {
// DEFAULT-NEXT:                                                         let %87 BgL_testz00_4238: i32 [storage=automatic];
// DEFAULT-NEXT:                                                         {
// DEFAULT-NEXT:                                                             let %88 BgL_auxz00_4239: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:                                                             write<ptr<@type0>>(%88, read<ptr<@type0>>(field0(field0(deref(int_to_ptr<ptr<@type0>, reason=explicit>(sub<i64, overflow=ub>(ptr_to_int<i64, reason=explicit>(read<ptr<@type0>>(%11)), widen<i64, reason=usual_arith>(const<i32>(3)))))))));
// DEFAULT-NEXT:                                                             write<i32>(%87, from_bool<i32, reason=assign>(eq<i64>(and<i64>(ptr_to_int<i64, reason=explicit>(read<ptr<@type0>>(%88)), widen<i64, reason=usual_arith>(sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(2)), const<i32>(1)))), widen<i64, reason=usual_arith>(const<i32>(3)))));
// DEFAULT-NEXT:                                                         }
// DEFAULT-NEXT:                                                         if ne<i32>(read<i32>(%87), const<i32>(0))
// DEFAULT-NEXT:                                                             {
// DEFAULT-NEXT:                                                                 goto %10;
// DEFAULT-NEXT:                                                             }
// DEFAULT-NEXT:                                                         else
// DEFAULT-NEXT:                                                             {
// DEFAULT-NEXT:                                                                 let %89 BgL_testz00_4242: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                 {
// DEFAULT-NEXT:                                                                     let %90 BgL_auxz00_4243: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:                                                                     write<ptr<@type0>>(%90, read<ptr<@type0>>(field0(field0(deref(int_to_ptr<ptr<@type0>, reason=explicit>(sub<i64, overflow=ub>(ptr_to_int<i64, reason=explicit>(read<ptr<@type0>>(%33)), widen<i64, reason=usual_arith>(const<i32>(3)))))))));
// DEFAULT-NEXT:                                                                     write<i32>(%89, from_bool<i32, reason=assign>(eq<i64>(and<i64>(ptr_to_int<i64, reason=explicit>(read<ptr<@type0>>(%90)), widen<i64, reason=usual_arith>(sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(2)), const<i32>(1)))), widen<i64, reason=usual_arith>(const<i32>(3)))));
// DEFAULT-NEXT:                                                                 }
// DEFAULT-NEXT:                                                                 if ne<i32>(read<i32>(%89), const<i32>(0))
// DEFAULT-NEXT:                                                                     {
// DEFAULT-NEXT:                                                                         goto %10;
// DEFAULT-NEXT:                                                                     }
// DEFAULT-NEXT:                                                                 else
// DEFAULT-NEXT:                                                                     {
// DEFAULT-NEXT:                                                                         if ne<ptr<@type0>>(read<ptr<@type0>>(%14), int_to_ptr<ptr<@type0>, reason=explicit>(or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i64, reason=explicit>(const<i32>(1)), const<i32>(2)), widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:                                                                             {
// DEFAULT-NEXT:                                                                                 let %91 BgL_v1048z00_1018: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:                                                                                 {
// DEFAULT-NEXT:                                                                                     let %92 BgL_auxz00_4248: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                                     write<i32>(%92, truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(3))));
// DEFAULT-NEXT:                                                                                     write<ptr<@type0>>(%91, call<ptr<@type0>, signature=fn(i32) -> ptr<@type0>>(%4, read<i32>(%92)));
// DEFAULT-NEXT:                                                                                     call<ptr<@type0>, signature=fn(i32) -> ptr<@type0>>(%4, read<i32>(%92));
// DEFAULT-NEXT:                                                                                 }
// DEFAULT-NEXT:                                                                                 {
// DEFAULT-NEXT:                                                                                     let %93 BgL_arg1602z00_1020: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:                                                                                     write<ptr<@type0>>(%93, call<ptr<@type0>, signature=fn(ptr<@type0>, ptr<@type0>) -> ptr<@type0>>(%5, read<ptr<@type0>>(%13), read<ptr<@type0>>(%12)));
// DEFAULT-NEXT:                                                                                     call<ptr<@type0>, signature=fn(ptr<@type0>, ptr<@type0>) -> ptr<@type0>>(%5, read<ptr<@type0>>(%13), read<ptr<@type0>>(%12));
// DEFAULT-NEXT:                                                                                     {
// DEFAULT-NEXT:                                                                                         let %94 BgL_auxz00_4252: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                                         write<i32>(%94, truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))));
// DEFAULT-NEXT:                                                                                         write<ptr<@type0>>(deref(ptr_offset<ptr<ptr<@type0>>, subtract=false, element=ptr<@type0>, overflow=ub>(addr_of<ptr<ptr<@type0>>>(field2(field1(deref(read<ptr<@type0>>(%91))))), read<i32>(%94))), read<ptr<@type0>>(%93));
// DEFAULT-NEXT:                                                                                     }
// DEFAULT-NEXT:                                                                                 }
// DEFAULT-NEXT:                                                                                 {
// DEFAULT-NEXT:                                                                                     let %95 BgL_auxz00_4255: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                                     write<i32>(%95, truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))));
// DEFAULT-NEXT:                                                                                     write<ptr<@type0>>(deref(ptr_offset<ptr<ptr<@type0>>, subtract=false, element=ptr<@type0>, overflow=ub>(addr_of<ptr<ptr<@type0>>>(field2(field1(deref(read<ptr<@type0>>(%91))))), read<i32>(%95))), read<ptr<@type0>>(%15));
// DEFAULT-NEXT:                                                                                 }
// DEFAULT-NEXT:                                                                                 {
// DEFAULT-NEXT:                                                                                     let %96 BgL_auxz00_4260: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:                                                                                     let %97 BgL_auxz00_4258: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                                     write<ptr<@type0>>(%96, int_to_ptr<ptr<@type0>, reason=explicit>(or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i64, reason=explicit>(const<i32>(49)), const<i32>(2)), widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                                                                     write<i32>(%97, truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))));
// DEFAULT-NEXT:                                                                                     write<ptr<@type0>>(deref(ptr_offset<ptr<ptr<@type0>>, subtract=false, element=ptr<@type0>, overflow=ub>(addr_of<ptr<ptr<@type0>>>(field2(field1(deref(read<ptr<@type0>>(%91))))), read<i32>(%97))), read<ptr<@type0>>(%96));
// DEFAULT-NEXT:                                                                                 }
// DEFAULT-NEXT:                                                                                 return read<ptr<@type0>>(%91);
// DEFAULT-NEXT:                                                                             }
// DEFAULT-NEXT:                                                                         else
// DEFAULT-NEXT:                                                                             {
// DEFAULT-NEXT:                                                                                 let %98 BgL_v1049z00_1021: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:                                                                                 {
// DEFAULT-NEXT:                                                                                     let %99 BgL_auxz00_4263: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                                     write<i32>(%99, truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(3))));
// DEFAULT-NEXT:                                                                                     write<ptr<@type0>>(%98, call<ptr<@type0>, signature=fn(i32) -> ptr<@type0>>(%4, read<i32>(%99)));
// DEFAULT-NEXT:                                                                                     call<ptr<@type0>, signature=fn(i32) -> ptr<@type0>>(%4, read<i32>(%99));
// DEFAULT-NEXT:                                                                                 }
// DEFAULT-NEXT:                                                                                 {
// DEFAULT-NEXT:                                                                                     let %100 BgL_auxz00_4266: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                                     write<i32>(%100, truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))));
// DEFAULT-NEXT:                                                                                     write<ptr<@type0>>(deref(ptr_offset<ptr<ptr<@type0>>, subtract=false, element=ptr<@type0>, overflow=ub>(addr_of<ptr<ptr<@type0>>>(field2(field1(deref(read<ptr<@type0>>(%98))))), read<i32>(%100))), read<ptr<@type0>>(%12));
// DEFAULT-NEXT:                                                                                 }
// DEFAULT-NEXT:                                                                                 {
// DEFAULT-NEXT:                                                                                     let %101 BgL_auxz00_4269: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                                     write<i32>(%101, truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))));
// DEFAULT-NEXT:                                                                                     write<ptr<@type0>>(deref(ptr_offset<ptr<ptr<@type0>>, subtract=false, element=ptr<@type0>, overflow=ub>(addr_of<ptr<ptr<@type0>>>(field2(field1(deref(read<ptr<@type0>>(%98))))), read<i32>(%101))), read<ptr<@type0>>(%15));
// DEFAULT-NEXT:                                                                                 }
// DEFAULT-NEXT:                                                                                 {
// DEFAULT-NEXT:                                                                                     let %102 BgL_auxz00_4274: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:                                                                                     let %103 BgL_auxz00_4272: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                                     write<ptr<@type0>>(%102, int_to_ptr<ptr<@type0>, reason=explicit>(or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i64, reason=explicit>(const<i32>(53)), const<i32>(2)), widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                                                                     write<i32>(%103, truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))));
// DEFAULT-NEXT:                                                                                     write<ptr<@type0>>(deref(ptr_offset<ptr<ptr<@type0>>, subtract=false, element=ptr<@type0>, overflow=ub>(addr_of<ptr<ptr<@type0>>>(field2(field1(deref(read<ptr<@type0>>(%98))))), read<i32>(%103))), read<ptr<@type0>>(%102));
// DEFAULT-NEXT:                                                                                 }
// DEFAULT-NEXT:                                                                                 return read<ptr<@type0>>(%98);
// DEFAULT-NEXT:                                                                             }
// DEFAULT-NEXT:                                                                     }
// DEFAULT-NEXT:                                                             }
// DEFAULT-NEXT:                                                     }
// DEFAULT-NEXT:                                             }
// DEFAULT-NEXT:                                     }
// DEFAULT-NEXT:                                 else
// DEFAULT-NEXT:                                     {
// DEFAULT-NEXT:                                         let %104 BgL_testz00_4277: i32 [storage=automatic];
// DEFAULT-NEXT:                                         {
// DEFAULT-NEXT:                                             let %105 BgL_auxz00_4278: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:                                             write<ptr<@type0>>(%105, read<ptr<@type0>>(field0(field0(deref(int_to_ptr<ptr<@type0>, reason=explicit>(sub<i64, overflow=ub>(ptr_to_int<i64, reason=explicit>(read<ptr<@type0>>(%11)), widen<i64, reason=usual_arith>(const<i32>(3)))))))));
// DEFAULT-NEXT:                                             write<i32>(%104, from_bool<i32, reason=assign>(eq<i64>(and<i64>(ptr_to_int<i64, reason=explicit>(read<ptr<@type0>>(%105)), widen<i64, reason=usual_arith>(sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(2)), const<i32>(1)))), widen<i64, reason=usual_arith>(const<i32>(3)))));
// DEFAULT-NEXT:                                         }
// DEFAULT-NEXT:                                         if ne<i32>(read<i32>(%104), const<i32>(0))
// DEFAULT-NEXT:                                             {
// DEFAULT-NEXT:                                                 goto %10;
// DEFAULT-NEXT:                                             }
// DEFAULT-NEXT:                                         else
// DEFAULT-NEXT:                                             {
// DEFAULT-NEXT:                                                 if ne<ptr<@type0>>(read<ptr<@type0>>(%14), int_to_ptr<ptr<@type0>, reason=explicit>(or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i64, reason=explicit>(const<i32>(1)), const<i32>(2)), widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:                                                     {
// DEFAULT-NEXT:                                                         let %106 BgL_v1046z00_1014: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:                                                         {
// DEFAULT-NEXT:                                                             let %107 BgL_auxz00_4283: i32 [storage=automatic];
// DEFAULT-NEXT:                                                             write<i32>(%107, truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(3))));
// DEFAULT-NEXT:                                                             write<ptr<@type0>>(%106, call<ptr<@type0>, signature=fn(i32) -> ptr<@type0>>(%4, read<i32>(%107)));
// DEFAULT-NEXT:                                                             call<ptr<@type0>, signature=fn(i32) -> ptr<@type0>>(%4, read<i32>(%107));
// DEFAULT-NEXT:                                                         }
// DEFAULT-NEXT:                                                         {
// DEFAULT-NEXT:                                                             let %108 BgL_arg1600z00_1016: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:                                                             write<ptr<@type0>>(%108, call<ptr<@type0>, signature=fn(ptr<@type0>, ptr<@type0>) -> ptr<@type0>>(%5, read<ptr<@type0>>(%13), read<ptr<@type0>>(%12)));
// DEFAULT-NEXT:                                                             call<ptr<@type0>, signature=fn(ptr<@type0>, ptr<@type0>) -> ptr<@type0>>(%5, read<ptr<@type0>>(%13), read<ptr<@type0>>(%12));
// DEFAULT-NEXT:                                                             {
// DEFAULT-NEXT:                                                                 let %109 BgL_auxz00_4287: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                 write<i32>(%109, truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))));
// DEFAULT-NEXT:                                                                 write<ptr<@type0>>(deref(ptr_offset<ptr<ptr<@type0>>, subtract=false, element=ptr<@type0>, overflow=ub>(addr_of<ptr<ptr<@type0>>>(field2(field1(deref(read<ptr<@type0>>(%106))))), read<i32>(%109))), read<ptr<@type0>>(%108));
// DEFAULT-NEXT:                                                             }
// DEFAULT-NEXT:                                                         }
// DEFAULT-NEXT:                                                         {
// DEFAULT-NEXT:                                                             let %110 BgL_auxz00_4290: i32 [storage=automatic];
// DEFAULT-NEXT:                                                             write<i32>(%110, truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))));
// DEFAULT-NEXT:                                                             write<ptr<@type0>>(deref(ptr_offset<ptr<ptr<@type0>>, subtract=false, element=ptr<@type0>, overflow=ub>(addr_of<ptr<ptr<@type0>>>(field2(field1(deref(read<ptr<@type0>>(%106))))), read<i32>(%110))), read<ptr<@type0>>(%15));
// DEFAULT-NEXT:                                                         }
// DEFAULT-NEXT:                                                         {
// DEFAULT-NEXT:                                                             let %111 BgL_auxz00_4295: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:                                                             let %112 BgL_auxz00_4293: i32 [storage=automatic];
// DEFAULT-NEXT:                                                             write<ptr<@type0>>(%111, int_to_ptr<ptr<@type0>, reason=explicit>(or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i64, reason=explicit>(const<i32>(48)), const<i32>(2)), widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                                             write<i32>(%112, truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))));
// DEFAULT-NEXT:                                                             write<ptr<@type0>>(deref(ptr_offset<ptr<ptr<@type0>>, subtract=false, element=ptr<@type0>, overflow=ub>(addr_of<ptr<ptr<@type0>>>(field2(field1(deref(read<ptr<@type0>>(%106))))), read<i32>(%112))), read<ptr<@type0>>(%111));
// DEFAULT-NEXT:                                                         }
// DEFAULT-NEXT:                                                         return read<ptr<@type0>>(%106);
// DEFAULT-NEXT:                                                     }
// DEFAULT-NEXT:                                                 else
// DEFAULT-NEXT:                                                     {
// DEFAULT-NEXT:                                                         let %113 BgL_v1047z00_1017: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:                                                         {
// DEFAULT-NEXT:                                                             let %114 BgL_auxz00_4298: i32 [storage=automatic];
// DEFAULT-NEXT:                                                             write<i32>(%114, truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(3))));
// DEFAULT-NEXT:                                                             write<ptr<@type0>>(%113, call<ptr<@type0>, signature=fn(i32) -> ptr<@type0>>(%4, read<i32>(%114)));
// DEFAULT-NEXT:                                                             call<ptr<@type0>, signature=fn(i32) -> ptr<@type0>>(%4, read<i32>(%114));
// DEFAULT-NEXT:                                                         }
// DEFAULT-NEXT:                                                         {
// DEFAULT-NEXT:                                                             let %115 BgL_auxz00_4301: i32 [storage=automatic];
// DEFAULT-NEXT:                                                             write<i32>(%115, truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))));
// DEFAULT-NEXT:                                                             write<ptr<@type0>>(deref(ptr_offset<ptr<ptr<@type0>>, subtract=false, element=ptr<@type0>, overflow=ub>(addr_of<ptr<ptr<@type0>>>(field2(field1(deref(read<ptr<@type0>>(%113))))), read<i32>(%115))), read<ptr<@type0>>(%12));
// DEFAULT-NEXT:                                                         }
// DEFAULT-NEXT:                                                         {
// DEFAULT-NEXT:                                                             let %116 BgL_auxz00_4304: i32 [storage=automatic];
// DEFAULT-NEXT:                                                             write<i32>(%116, truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))));
// DEFAULT-NEXT:                                                             write<ptr<@type0>>(deref(ptr_offset<ptr<ptr<@type0>>, subtract=false, element=ptr<@type0>, overflow=ub>(addr_of<ptr<ptr<@type0>>>(field2(field1(deref(read<ptr<@type0>>(%113))))), read<i32>(%116))), read<ptr<@type0>>(%15));
// DEFAULT-NEXT:                                                         }
// DEFAULT-NEXT:                                                         {
// DEFAULT-NEXT:                                                             let %117 BgL_auxz00_4309: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:                                                             let %118 BgL_auxz00_4307: i32 [storage=automatic];
// DEFAULT-NEXT:                                                             write<ptr<@type0>>(%117, int_to_ptr<ptr<@type0>, reason=explicit>(or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i64, reason=explicit>(const<i32>(52)), const<i32>(2)), widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                                             write<i32>(%118, truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))));
// DEFAULT-NEXT:                                                             write<ptr<@type0>>(deref(ptr_offset<ptr<ptr<@type0>>, subtract=false, element=ptr<@type0>, overflow=ub>(addr_of<ptr<ptr<@type0>>>(field2(field1(deref(read<ptr<@type0>>(%113))))), read<i32>(%118))), read<ptr<@type0>>(%117));
// DEFAULT-NEXT:                                                         }
// DEFAULT-NEXT:                                                         return read<ptr<@type0>>(%113);
// DEFAULT-NEXT:                                                     }
// DEFAULT-NEXT:                                             }
// DEFAULT-NEXT:                                     }
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<ptr<@type0>>(read<ptr<@type0>>(%14), int_to_ptr<ptr<@type0>, reason=explicit>(or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i64, reason=explicit>(const<i32>(1)), const<i32>(2)), widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %119 BgL_v1044z00_1010: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     let %120 BgL_auxz00_4314: i32 [storage=automatic];
// DEFAULT-NEXT:                                     write<i32>(%120, truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(3))));
// DEFAULT-NEXT:                                     write<ptr<@type0>>(%119, call<ptr<@type0>, signature=fn(i32) -> ptr<@type0>>(%4, read<i32>(%120)));
// DEFAULT-NEXT:                                     call<ptr<@type0>, signature=fn(i32) -> ptr<@type0>>(%4, read<i32>(%120));
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     let %121 BgL_arg1598z00_1012: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:                                     write<ptr<@type0>>(%121, call<ptr<@type0>, signature=fn(ptr<@type0>, ptr<@type0>) -> ptr<@type0>>(%5, read<ptr<@type0>>(%13), read<ptr<@type0>>(%12)));
// DEFAULT-NEXT:                                     call<ptr<@type0>, signature=fn(ptr<@type0>, ptr<@type0>) -> ptr<@type0>>(%5, read<ptr<@type0>>(%13), read<ptr<@type0>>(%12));
// DEFAULT-NEXT:                                     {
// DEFAULT-NEXT:                                         let %122 BgL_auxz00_4318: i32 [storage=automatic];
// DEFAULT-NEXT:                                         write<i32>(%122, truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))));
// DEFAULT-NEXT:                                         write<ptr<@type0>>(deref(ptr_offset<ptr<ptr<@type0>>, subtract=false, element=ptr<@type0>, overflow=ub>(addr_of<ptr<ptr<@type0>>>(field2(field1(deref(read<ptr<@type0>>(%119))))), read<i32>(%122))), read<ptr<@type0>>(%121));
// DEFAULT-NEXT:                                     }
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     let %123 BgL_auxz00_4321: i32 [storage=automatic];
// DEFAULT-NEXT:                                     write<i32>(%123, truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))));
// DEFAULT-NEXT:                                     write<ptr<@type0>>(deref(ptr_offset<ptr<ptr<@type0>>, subtract=false, element=ptr<@type0>, overflow=ub>(addr_of<ptr<ptr<@type0>>>(field2(field1(deref(read<ptr<@type0>>(%119))))), read<i32>(%123))), read<ptr<@type0>>(%15));
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     let %124 BgL_auxz00_4326: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:                                     let %125 BgL_auxz00_4324: i32 [storage=automatic];
// DEFAULT-NEXT:                                     write<ptr<@type0>>(%124, int_to_ptr<ptr<@type0>, reason=explicit>(or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i64, reason=explicit>(const<i32>(47)), const<i32>(2)), widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                     write<i32>(%125, truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))));
// DEFAULT-NEXT:                                     write<ptr<@type0>>(deref(ptr_offset<ptr<ptr<@type0>>, subtract=false, element=ptr<@type0>, overflow=ub>(addr_of<ptr<ptr<@type0>>>(field2(field1(deref(read<ptr<@type0>>(%119))))), read<i32>(%125))), read<ptr<@type0>>(%124));
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 return read<ptr<@type0>>(%119);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %126 BgL_v1045z00_1013: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     let %127 BgL_auxz00_4329: i32 [storage=automatic];
// DEFAULT-NEXT:                                     write<i32>(%127, truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(3))));
// DEFAULT-NEXT:                                     write<ptr<@type0>>(%126, call<ptr<@type0>, signature=fn(i32) -> ptr<@type0>>(%4, read<i32>(%127)));
// DEFAULT-NEXT:                                     call<ptr<@type0>, signature=fn(i32) -> ptr<@type0>>(%4, read<i32>(%127));
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     let %128 BgL_auxz00_4332: i32 [storage=automatic];
// DEFAULT-NEXT:                                     write<i32>(%128, truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))));
// DEFAULT-NEXT:                                     write<ptr<@type0>>(deref(ptr_offset<ptr<ptr<@type0>>, subtract=false, element=ptr<@type0>, overflow=ub>(addr_of<ptr<ptr<@type0>>>(field2(field1(deref(read<ptr<@type0>>(%126))))), read<i32>(%128))), read<ptr<@type0>>(%12));
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     let %129 BgL_auxz00_4335: i32 [storage=automatic];
// DEFAULT-NEXT:                                     write<i32>(%129, truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))));
// DEFAULT-NEXT:                                     write<ptr<@type0>>(deref(ptr_offset<ptr<ptr<@type0>>, subtract=false, element=ptr<@type0>, overflow=ub>(addr_of<ptr<ptr<@type0>>>(field2(field1(deref(read<ptr<@type0>>(%126))))), read<i32>(%129))), read<ptr<@type0>>(%15));
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     let %130 BgL_auxz00_4340: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:                                     let %131 BgL_auxz00_4338: i32 [storage=automatic];
// DEFAULT-NEXT:                                     write<ptr<@type0>>(%130, int_to_ptr<ptr<@type0>, reason=explicit>(or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i64, reason=explicit>(const<i32>(51)), const<i32>(2)), widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                     write<i32>(%131, truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))));
// DEFAULT-NEXT:                                     write<ptr<@type0>>(deref(ptr_offset<ptr<ptr<@type0>>, subtract=false, element=ptr<@type0>, overflow=ub>(addr_of<ptr<ptr<@type0>>>(field2(field1(deref(read<ptr<@type0>>(%126))))), read<i32>(%131))), read<ptr<@type0>>(%130));
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 return read<ptr<@type0>>(%126);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
