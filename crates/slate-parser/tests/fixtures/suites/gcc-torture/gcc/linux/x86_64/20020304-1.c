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
// DEFAULT-NEXT:     type @type[[TYPE_scmobj:[0-9]+]] scmobj = union {
// DEFAULT-NEXT:         field0 pair_t: @type[[TYPE_pair:[0-9]+]];
// DEFAULT-NEXT:         field1 vector_t: @type[[TYPE_vector:[0-9]+]];
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE_pair]] pair = struct {
// DEFAULT-NEXT:         field0 car: ptr<@type[[TYPE_scmobj]]>;
// DEFAULT-NEXT:         field1 cdr: ptr<@type[[TYPE_scmobj]]>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_vector]] vector = struct {
// DEFAULT-NEXT:         field0 header: i64;
// DEFAULT-NEXT:         field1 length: i32;
// DEFAULT-NEXT:         field2 obj0: ptr<@type[[TYPE_scmobj]]>;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// DEFAULT-NEXT:     type @type[[TYPE_obj_t:[0-9]+]] obj_t = ptr<@type[[TYPE_scmobj]]>;
// DEFAULT-NEXT:     fn %[[VALUE_create_vector:[0-9]+]] @create_vector(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> ptr<@type[[TYPE_scmobj]]> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_make_pair:[0-9]+]] @make_pair(%[[VALUE1:[0-9]+]] <unnamed>: ptr<@type[[TYPE_scmobj]]>, %[[VALUE2:[0-9]+]] <unnamed>: ptr<@type[[TYPE_scmobj]]>) -> ptr<@type[[TYPE_scmobj]]> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_bgl_list_length:[0-9]+]] @bgl_list_length(%[[VALUE3:[0-9]+]] <unnamed>: ptr<@type[[TYPE_scmobj]]>) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_BGl_equalzf3zf3zz__r4_equivalence_6_2z00:[0-9]+]] @BGl_equalzf3zf3zz__r4_equivalence_6_2z00(%[[VALUE4:[0-9]+]] <unnamed>: ptr<@type[[TYPE_scmobj]]>, %[[VALUE5:[0-9]+]] <unnamed>: ptr<@type[[TYPE_scmobj]]>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_BGl_evcompilezd2lambdazd2zz__evcompilez00:[0-9]+]] @BGl_evcompilezd2lambdazd2zz__evcompilez00(%[[VALUE_BgL_formalsz00_39:[0-9]+]] BgL_formalsz00_39: ptr<@type[[TYPE_scmobj]]>, %[[VALUE_BgL_bodyz00_40:[0-9]+]] BgL_bodyz00_40: ptr<@type[[TYPE_scmobj]]>, %[[VALUE_BgL_wherez00_41:[0-9]+]] BgL_wherez00_41: ptr<@type[[TYPE_scmobj]]>, %[[VALUE_BgL_namedzf3zf3_42:[0-9]+]] BgL_namedzf3zf3_42: ptr<@type[[TYPE_scmobj]]>, %[[VALUE_BgL_locz00_43:[0-9]+]] BgL_locz00_43: ptr<@type[[TYPE_scmobj]]>) -> ptr<@type[[TYPE_scmobj]]> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type[[TYPE_scmobj]]>, ptr<@type[[TYPE_scmobj]]>) -> i32>(%[[VALUE_BGl_equalzf3zf3zz__r4_equivalence_6_2z00]], read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_formalsz00_39]]), int_to_ptr<ptr<@type[[TYPE_scmobj]]>, reason=explicit>(or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i64, reason=explicit>(const<i32>(0)), const<i32>(2)), widen<i64, reason=usual_arith>(const<i32>(2))))), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 label %[[VALUE_BgL_tagzd21966zd2_943:[0-9]+]] BgL_tagzd21966zd2_943:
// DEFAULT-NEXT:                     if ne<ptr<@type[[TYPE_scmobj]]>>(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_namedzf3zf3_42]]), int_to_ptr<ptr<@type[[TYPE_scmobj]]>, reason=explicit>(or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i64, reason=explicit>(const<i32>(1)), const<i32>(2)), widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             let %[[VALUE_BgL_v1042z00_998:[0-9]+]] BgL_v1042z00_998: ptr<@type[[TYPE_scmobj]]> [storage=automatic];
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE_BgL_auxz00_4066:[0-9]+]] BgL_auxz00_4066: i32 [storage=automatic];
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_BgL_auxz00_4066]], truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(3))));
// DEFAULT-NEXT:                                 write<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_v1042z00_998]], call<ptr<@type[[TYPE_scmobj]]>, signature=fn(i32) -> ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_create_vector]], read<i32>(%[[VALUE_BgL_auxz00_4066]])));
// DEFAULT-NEXT:                                 call<ptr<@type[[TYPE_scmobj]]>, signature=fn(i32) -> ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_create_vector]], read<i32>(%[[VALUE_BgL_auxz00_4066]]));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE_BgL_arg1586z00_1000:[0-9]+]] BgL_arg1586z00_1000: ptr<@type[[TYPE_scmobj]]> [storage=automatic];
// DEFAULT-NEXT:                                 write<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_arg1586z00_1000]], call<ptr<@type[[TYPE_scmobj]]>, signature=fn(ptr<@type[[TYPE_scmobj]]>, ptr<@type[[TYPE_scmobj]]>) -> ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_make_pair]], read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_wherez00_41]]), read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_bodyz00_40]])));
// DEFAULT-NEXT:                                 call<ptr<@type[[TYPE_scmobj]]>, signature=fn(ptr<@type[[TYPE_scmobj]]>, ptr<@type[[TYPE_scmobj]]>) -> ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_make_pair]], read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_wherez00_41]]), read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_bodyz00_40]]));
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     let %[[VALUE_BgL_auxz00_4070:[0-9]+]] BgL_auxz00_4070: i32 [storage=automatic];
// DEFAULT-NEXT:                                     write<i32>(%[[VALUE_BgL_auxz00_4070]], truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))));
// DEFAULT-NEXT:                                     write<ptr<@type[[TYPE_scmobj]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_scmobj]]>>, subtract=false, element=ptr<@type[[TYPE_scmobj]]>, overflow=ub>(addr_of<ptr<ptr<@type[[TYPE_scmobj]]>>>(field2(field1(deref(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_v1042z00_998]]))))), read<i32>(%[[VALUE_BgL_auxz00_4070]]))), read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_arg1586z00_1000]]));
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE_BgL_auxz00_4073:[0-9]+]] BgL_auxz00_4073: i32 [storage=automatic];
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_BgL_auxz00_4073]], truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))));
// DEFAULT-NEXT:                                 write<ptr<@type[[TYPE_scmobj]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_scmobj]]>>, subtract=false, element=ptr<@type[[TYPE_scmobj]]>, overflow=ub>(addr_of<ptr<ptr<@type[[TYPE_scmobj]]>>>(field2(field1(deref(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_v1042z00_998]]))))), read<i32>(%[[VALUE_BgL_auxz00_4073]]))), read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_locz00_43]]));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE_BgL_auxz00_4078:[0-9]+]] BgL_auxz00_4078: ptr<@type[[TYPE_scmobj]]> [storage=automatic];
// DEFAULT-NEXT:                                 let %[[VALUE_BgL_auxz00_4076:[0-9]+]] BgL_auxz00_4076: i32 [storage=automatic];
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     let %[[VALUE_BgL_auxz00_4079:[0-9]+]] BgL_auxz00_4079: i64 [storage=automatic];
// DEFAULT-NEXT:                                     {
// DEFAULT-NEXT:                                         let %[[VALUE_BgL_auxz00_4080:[0-9]+]] BgL_auxz00_4080: i64 [storage=automatic];
// DEFAULT-NEXT:                                         write<i64>(%[[VALUE_BgL_auxz00_4080]], call<i64, signature=fn(ptr<@type[[TYPE_scmobj]]>) -> i64>(%[[VALUE_bgl_list_length]], read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_formalsz00_39]])));
// DEFAULT-NEXT:                                         call<i64, signature=fn(ptr<@type[[TYPE_scmobj]]>) -> i64>(%[[VALUE_bgl_list_length]], read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_formalsz00_39]]));
// DEFAULT-NEXT:                                         write<i64>(%[[VALUE_BgL_auxz00_4079]], add<i64, overflow=ub>(read<i64>(%[[VALUE_BgL_auxz00_4080]]), widen<i64, reason=explicit>(const<i32>(37))));
// DEFAULT-NEXT:                                     }
// DEFAULT-NEXT:                                     write<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_auxz00_4078]], int_to_ptr<ptr<@type[[TYPE_scmobj]]>, reason=explicit>(or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i64>(%[[VALUE_BgL_auxz00_4079]]), const<i32>(2)), widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_BgL_auxz00_4076]], truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))));
// DEFAULT-NEXT:                                 write<ptr<@type[[TYPE_scmobj]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_scmobj]]>>, subtract=false, element=ptr<@type[[TYPE_scmobj]]>, overflow=ub>(addr_of<ptr<ptr<@type[[TYPE_scmobj]]>>>(field2(field1(deref(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_v1042z00_998]]))))), read<i32>(%[[VALUE_BgL_auxz00_4076]]))), read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_auxz00_4078]]));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                             return read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_v1042z00_998]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             let %[[VALUE_BgL_v1043z00_1005:[0-9]+]] BgL_v1043z00_1005: ptr<@type[[TYPE_scmobj]]> [storage=automatic];
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE_BgL_auxz00_4085:[0-9]+]] BgL_auxz00_4085: i32 [storage=automatic];
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_BgL_auxz00_4085]], truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(3))));
// DEFAULT-NEXT:                                 write<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_v1043z00_1005]], call<ptr<@type[[TYPE_scmobj]]>, signature=fn(i32) -> ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_create_vector]], read<i32>(%[[VALUE_BgL_auxz00_4085]])));
// DEFAULT-NEXT:                                 call<ptr<@type[[TYPE_scmobj]]>, signature=fn(i32) -> ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_create_vector]], read<i32>(%[[VALUE_BgL_auxz00_4085]]));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE_BgL_auxz00_4088:[0-9]+]] BgL_auxz00_4088: i32 [storage=automatic];
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_BgL_auxz00_4088]], truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))));
// DEFAULT-NEXT:                                 write<ptr<@type[[TYPE_scmobj]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_scmobj]]>>, subtract=false, element=ptr<@type[[TYPE_scmobj]]>, overflow=ub>(addr_of<ptr<ptr<@type[[TYPE_scmobj]]>>>(field2(field1(deref(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_v1043z00_1005]]))))), read<i32>(%[[VALUE_BgL_auxz00_4088]]))), read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_bodyz00_40]]));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE_BgL_auxz00_4091:[0-9]+]] BgL_auxz00_4091: i32 [storage=automatic];
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_BgL_auxz00_4091]], truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))));
// DEFAULT-NEXT:                                 write<ptr<@type[[TYPE_scmobj]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_scmobj]]>>, subtract=false, element=ptr<@type[[TYPE_scmobj]]>, overflow=ub>(addr_of<ptr<ptr<@type[[TYPE_scmobj]]>>>(field2(field1(deref(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_v1043z00_1005]]))))), read<i32>(%[[VALUE_BgL_auxz00_4091]]))), read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_locz00_43]]));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE_BgL_auxz00_4096:[0-9]+]] BgL_auxz00_4096: ptr<@type[[TYPE_scmobj]]> [storage=automatic];
// DEFAULT-NEXT:                                 let %[[VALUE_BgL_auxz00_4094:[0-9]+]] BgL_auxz00_4094: i32 [storage=automatic];
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     let %[[VALUE_BgL_auxz00_4097:[0-9]+]] BgL_auxz00_4097: i64 [storage=automatic];
// DEFAULT-NEXT:                                     {
// DEFAULT-NEXT:                                         let %[[VALUE_BgL_auxz00_4098:[0-9]+]] BgL_auxz00_4098: i64 [storage=automatic];
// DEFAULT-NEXT:                                         write<i64>(%[[VALUE_BgL_auxz00_4098]], call<i64, signature=fn(ptr<@type[[TYPE_scmobj]]>) -> i64>(%[[VALUE_bgl_list_length]], read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_formalsz00_39]])));
// DEFAULT-NEXT:                                         call<i64, signature=fn(ptr<@type[[TYPE_scmobj]]>) -> i64>(%[[VALUE_bgl_list_length]], read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_formalsz00_39]]));
// DEFAULT-NEXT:                                         write<i64>(%[[VALUE_BgL_auxz00_4097]], add<i64, overflow=ub>(read<i64>(%[[VALUE_BgL_auxz00_4098]]), widen<i64, reason=explicit>(const<i32>(42))));
// DEFAULT-NEXT:                                     }
// DEFAULT-NEXT:                                     write<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_auxz00_4096]], int_to_ptr<ptr<@type[[TYPE_scmobj]]>, reason=explicit>(or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i64>(%[[VALUE_BgL_auxz00_4097]]), const<i32>(2)), widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_BgL_auxz00_4094]], truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))));
// DEFAULT-NEXT:                                 write<ptr<@type[[TYPE_scmobj]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_scmobj]]>>, subtract=false, element=ptr<@type[[TYPE_scmobj]]>, overflow=ub>(addr_of<ptr<ptr<@type[[TYPE_scmobj]]>>>(field2(field1(deref(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_v1043z00_1005]]))))), read<i32>(%[[VALUE_BgL_auxz00_4094]]))), read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_auxz00_4096]]));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                             return read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_v1043z00_1005]]);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if eq<i64>(and<i64>(ptr_to_int<i64, reason=explicit>(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_formalsz00_39]])), widen<i64, reason=usual_arith>(sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(2)), const<i32>(1)))), widen<i64, reason=usual_arith>(const<i32>(3)))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<i32>(call<i32, signature=fn(ptr<@type[[TYPE_scmobj]]>, ptr<@type[[TYPE_scmobj]]>) -> i32>(%[[VALUE_BGl_equalzf3zf3zz__r4_equivalence_6_2z00]], read<ptr<@type[[TYPE_scmobj]]>>(field1(field0(deref(int_to_ptr<ptr<@type[[TYPE_scmobj]]>, reason=explicit>(sub<i64, overflow=ub>(ptr_to_int<i64, reason=explicit>(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_formalsz00_39]])), widen<i64, reason=usual_arith>(const<i32>(3)))))))), int_to_ptr<ptr<@type[[TYPE_scmobj]]>, reason=explicit>(or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i64, reason=explicit>(const<i32>(0)), const<i32>(2)), widen<i64, reason=usual_arith>(const<i32>(2))))), const<i32>(0))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 goto %[[VALUE_BgL_tagzd21966zd2_943]];
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE_BgL_cdrzd21979zd2_953:[0-9]+]] BgL_cdrzd21979zd2_953: ptr<@type[[TYPE_scmobj]]> [storage=automatic];
// DEFAULT-NEXT:                                 write<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_cdrzd21979zd2_953]], read<ptr<@type[[TYPE_scmobj]]>>(field1(field0(deref(int_to_ptr<ptr<@type[[TYPE_scmobj]]>, reason=explicit>(sub<i64, overflow=ub>(ptr_to_int<i64, reason=explicit>(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_formalsz00_39]])), widen<i64, reason=usual_arith>(const<i32>(3)))))))));
// DEFAULT-NEXT:                                 if eq<i64>(and<i64>(ptr_to_int<i64, reason=explicit>(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_cdrzd21979zd2_953]])), widen<i64, reason=usual_arith>(sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(2)), const<i32>(1)))), widen<i64, reason=usual_arith>(const<i32>(3)))
// DEFAULT-NEXT:                                     {
// DEFAULT-NEXT:                                         if ne<i32>(call<i32, signature=fn(ptr<@type[[TYPE_scmobj]]>, ptr<@type[[TYPE_scmobj]]>) -> i32>(%[[VALUE_BGl_equalzf3zf3zz__r4_equivalence_6_2z00]], read<ptr<@type[[TYPE_scmobj]]>>(field1(field0(deref(int_to_ptr<ptr<@type[[TYPE_scmobj]]>, reason=explicit>(sub<i64, overflow=ub>(ptr_to_int<i64, reason=explicit>(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_cdrzd21979zd2_953]])), widen<i64, reason=usual_arith>(const<i32>(3)))))))), int_to_ptr<ptr<@type[[TYPE_scmobj]]>, reason=explicit>(or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i64, reason=explicit>(const<i32>(0)), const<i32>(2)), widen<i64, reason=usual_arith>(const<i32>(2))))), const<i32>(0))
// DEFAULT-NEXT:                                             {
// DEFAULT-NEXT:                                                 goto %[[VALUE_BgL_tagzd21966zd2_943]];
// DEFAULT-NEXT:                                             }
// DEFAULT-NEXT:                                         else
// DEFAULT-NEXT:                                             {
// DEFAULT-NEXT:                                                 let %[[VALUE_BgL_cdrzd21986zd2_956:[0-9]+]] BgL_cdrzd21986zd2_956: ptr<@type[[TYPE_scmobj]]> [storage=automatic];
// DEFAULT-NEXT:                                                 write<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_cdrzd21986zd2_956]], read<ptr<@type[[TYPE_scmobj]]>>(field1(field0(deref(int_to_ptr<ptr<@type[[TYPE_scmobj]]>, reason=explicit>(sub<i64, overflow=ub>(ptr_to_int<i64, reason=explicit>(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_cdrzd21979zd2_953]])), widen<i64, reason=usual_arith>(const<i32>(3)))))))));
// DEFAULT-NEXT:                                                 if eq<i64>(and<i64>(ptr_to_int<i64, reason=explicit>(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_cdrzd21986zd2_956]])), widen<i64, reason=usual_arith>(sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(2)), const<i32>(1)))), widen<i64, reason=usual_arith>(const<i32>(3)))
// DEFAULT-NEXT:                                                     {
// DEFAULT-NEXT:                                                         if ne<i32>(call<i32, signature=fn(ptr<@type[[TYPE_scmobj]]>, ptr<@type[[TYPE_scmobj]]>) -> i32>(%[[VALUE_BGl_equalzf3zf3zz__r4_equivalence_6_2z00]], read<ptr<@type[[TYPE_scmobj]]>>(field1(field0(deref(int_to_ptr<ptr<@type[[TYPE_scmobj]]>, reason=explicit>(sub<i64, overflow=ub>(ptr_to_int<i64, reason=explicit>(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_cdrzd21986zd2_956]])), widen<i64, reason=usual_arith>(const<i32>(3)))))))), int_to_ptr<ptr<@type[[TYPE_scmobj]]>, reason=explicit>(or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i64, reason=explicit>(const<i32>(0)), const<i32>(2)), widen<i64, reason=usual_arith>(const<i32>(2))))), const<i32>(0))
// DEFAULT-NEXT:                                                             {
// DEFAULT-NEXT:                                                                 goto %[[VALUE_BgL_tagzd21966zd2_943]];
// DEFAULT-NEXT:                                                             }
// DEFAULT-NEXT:                                                         else
// DEFAULT-NEXT:                                                             {
// DEFAULT-NEXT:                                                                 let %[[VALUE_BgL_cdrzd21994zd2_959:[0-9]+]] BgL_cdrzd21994zd2_959: ptr<@type[[TYPE_scmobj]]> [storage=automatic];
// DEFAULT-NEXT:                                                                 {
// DEFAULT-NEXT:                                                                     let %[[VALUE_BgL_auxz00_4120:[0-9]+]] BgL_auxz00_4120: ptr<@type[[TYPE_scmobj]]> [storage=automatic];
// DEFAULT-NEXT:                                                                     write<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_auxz00_4120]], read<ptr<@type[[TYPE_scmobj]]>>(field1(field0(deref(int_to_ptr<ptr<@type[[TYPE_scmobj]]>, reason=explicit>(sub<i64, overflow=ub>(ptr_to_int<i64, reason=explicit>(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_cdrzd21979zd2_953]])), widen<i64, reason=usual_arith>(const<i32>(3)))))))));
// DEFAULT-NEXT:                                                                     write<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_cdrzd21994zd2_959]], read<ptr<@type[[TYPE_scmobj]]>>(field1(field0(deref(int_to_ptr<ptr<@type[[TYPE_scmobj]]>, reason=explicit>(sub<i64, overflow=ub>(ptr_to_int<i64, reason=explicit>(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_auxz00_4120]])), widen<i64, reason=usual_arith>(const<i32>(3)))))))));
// DEFAULT-NEXT:                                                                 }
// DEFAULT-NEXT:                                                                 if eq<i64>(and<i64>(ptr_to_int<i64, reason=explicit>(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_cdrzd21994zd2_959]])), widen<i64, reason=usual_arith>(sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(2)), const<i32>(1)))), widen<i64, reason=usual_arith>(const<i32>(3)))
// DEFAULT-NEXT:                                                                     {
// DEFAULT-NEXT:                                                                         if ne<i32>(call<i32, signature=fn(ptr<@type[[TYPE_scmobj]]>, ptr<@type[[TYPE_scmobj]]>) -> i32>(%[[VALUE_BGl_equalzf3zf3zz__r4_equivalence_6_2z00]], read<ptr<@type[[TYPE_scmobj]]>>(field1(field0(deref(int_to_ptr<ptr<@type[[TYPE_scmobj]]>, reason=explicit>(sub<i64, overflow=ub>(ptr_to_int<i64, reason=explicit>(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_cdrzd21994zd2_959]])), widen<i64, reason=usual_arith>(const<i32>(3)))))))), int_to_ptr<ptr<@type[[TYPE_scmobj]]>, reason=explicit>(or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i64, reason=explicit>(const<i32>(0)), const<i32>(2)), widen<i64, reason=usual_arith>(const<i32>(2))))), const<i32>(0))
// DEFAULT-NEXT:                                                                             {
// DEFAULT-NEXT:                                                                                 goto %[[VALUE_BgL_tagzd21966zd2_943]];
// DEFAULT-NEXT:                                                                             }
// DEFAULT-NEXT:                                                                         else
// DEFAULT-NEXT:                                                                             {
// DEFAULT-NEXT:                                                                                 let %[[VALUE_BgL_testz00_4128:[0-9]+]] BgL_testz00_4128: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                                 {
// DEFAULT-NEXT:                                                                                     let %[[VALUE_BgL_auxz00_4129:[0-9]+]] BgL_auxz00_4129: ptr<@type[[TYPE_scmobj]]> [storage=automatic];
// DEFAULT-NEXT:                                                                                     write<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_auxz00_4129]], read<ptr<@type[[TYPE_scmobj]]>>(field0(field0(deref(int_to_ptr<ptr<@type[[TYPE_scmobj]]>, reason=explicit>(sub<i64, overflow=ub>(ptr_to_int<i64, reason=explicit>(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_formalsz00_39]])), widen<i64, reason=usual_arith>(const<i32>(3)))))))));
// DEFAULT-NEXT:                                                                                     write<i32>(%[[VALUE_BgL_testz00_4128]], from_bool<i32, reason=assign>(eq<i64>(and<i64>(ptr_to_int<i64, reason=explicit>(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_auxz00_4129]])), widen<i64, reason=usual_arith>(sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(2)), const<i32>(1)))), widen<i64, reason=usual_arith>(const<i32>(3)))));
// DEFAULT-NEXT:                                                                                 }
// DEFAULT-NEXT:                                                                                 if ne<i32>(read<i32>(%[[VALUE_BgL_testz00_4128]]), const<i32>(0))
// DEFAULT-NEXT:                                                                                     {
// DEFAULT-NEXT:                                                                                         label %[[VALUE_BgL_tagzd21971zd2_948:[0-9]+]] BgL_tagzd21971zd2_948:
// DEFAULT-NEXT:                                                                                             if ne<ptr<@type[[TYPE_scmobj]]>>(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_namedzf3zf3_42]]), int_to_ptr<ptr<@type[[TYPE_scmobj]]>, reason=explicit>(or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i64, reason=explicit>(const<i32>(1)), const<i32>(2)), widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:                                                                                                 {
// DEFAULT-NEXT:                                                                                                     let %[[VALUE_BgL_v1052z00_1026:[0-9]+]] BgL_v1052z00_1026: ptr<@type[[TYPE_scmobj]]> [storage=automatic];
// DEFAULT-NEXT:                                                                                                     {
// DEFAULT-NEXT:                                                                                                         let %[[VALUE_BgL_auxz00_4134:[0-9]+]] BgL_auxz00_4134: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                                                         write<i32>(%[[VALUE_BgL_auxz00_4134]], truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(3))));
// DEFAULT-NEXT:                                                                                                         write<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_v1052z00_1026]], call<ptr<@type[[TYPE_scmobj]]>, signature=fn(i32) -> ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_create_vector]], read<i32>(%[[VALUE_BgL_auxz00_4134]])));
// DEFAULT-NEXT:                                                                                                         call<ptr<@type[[TYPE_scmobj]]>, signature=fn(i32) -> ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_create_vector]], read<i32>(%[[VALUE_BgL_auxz00_4134]]));
// DEFAULT-NEXT:                                                                                                     }
// DEFAULT-NEXT:                                                                                                     {
// DEFAULT-NEXT:                                                                                                         let %[[VALUE_BgL_arg1606z00_1028:[0-9]+]] BgL_arg1606z00_1028: ptr<@type[[TYPE_scmobj]]> [storage=automatic];
// DEFAULT-NEXT:                                                                                                         {
// DEFAULT-NEXT:                                                                                                             let %[[VALUE_BgL_v1053z00_1029:[0-9]+]] BgL_v1053z00_1029: ptr<@type[[TYPE_scmobj]]> [storage=automatic];
// DEFAULT-NEXT:                                                                                                             {
// DEFAULT-NEXT:                                                                                                                 let %[[VALUE_BgL_auxz00_4137:[0-9]+]] BgL_auxz00_4137: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                                                                 write<i32>(%[[VALUE_BgL_auxz00_4137]], truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(3))));
// DEFAULT-NEXT:                                                                                                                 write<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_v1053z00_1029]], call<ptr<@type[[TYPE_scmobj]]>, signature=fn(i32) -> ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_create_vector]], read<i32>(%[[VALUE_BgL_auxz00_4137]])));
// DEFAULT-NEXT:                                                                                                                 call<ptr<@type[[TYPE_scmobj]]>, signature=fn(i32) -> ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_create_vector]], read<i32>(%[[VALUE_BgL_auxz00_4137]]));
// DEFAULT-NEXT:                                                                                                             }
// DEFAULT-NEXT:                                                                                                             {
// DEFAULT-NEXT:                                                                                                                 let %[[VALUE_BgL_auxz00_4140:[0-9]+]] BgL_auxz00_4140: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                                                                 write<i32>(%[[VALUE_BgL_auxz00_4140]], truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))));
// DEFAULT-NEXT:                                                                                                                 write<ptr<@type[[TYPE_scmobj]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_scmobj]]>>, subtract=false, element=ptr<@type[[TYPE_scmobj]]>, overflow=ub>(addr_of<ptr<ptr<@type[[TYPE_scmobj]]>>>(field2(field1(deref(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_v1053z00_1029]]))))), read<i32>(%[[VALUE_BgL_auxz00_4140]]))), read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_formalsz00_39]]));
// DEFAULT-NEXT:                                                                                                             }
// DEFAULT-NEXT:                                                                                                             {
// DEFAULT-NEXT:                                                                                                                 let %[[VALUE_BgL_auxz00_4143:[0-9]+]] BgL_auxz00_4143: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                                                                 write<i32>(%[[VALUE_BgL_auxz00_4143]], truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))));
// DEFAULT-NEXT:                                                                                                                 write<ptr<@type[[TYPE_scmobj]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_scmobj]]>>, subtract=false, element=ptr<@type[[TYPE_scmobj]]>, overflow=ub>(addr_of<ptr<ptr<@type[[TYPE_scmobj]]>>>(field2(field1(deref(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_v1053z00_1029]]))))), read<i32>(%[[VALUE_BgL_auxz00_4143]]))), read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_bodyz00_40]]));
// DEFAULT-NEXT:                                                                                                             }
// DEFAULT-NEXT:                                                                                                             {
// DEFAULT-NEXT:                                                                                                                 let %[[VALUE_BgL_auxz00_4146:[0-9]+]] BgL_auxz00_4146: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                                                                 write<i32>(%[[VALUE_BgL_auxz00_4146]], truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))));
// DEFAULT-NEXT:                                                                                                                 write<ptr<@type[[TYPE_scmobj]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_scmobj]]>>, subtract=false, element=ptr<@type[[TYPE_scmobj]]>, overflow=ub>(addr_of<ptr<ptr<@type[[TYPE_scmobj]]>>>(field2(field1(deref(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_v1053z00_1029]]))))), read<i32>(%[[VALUE_BgL_auxz00_4146]]))), read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_wherez00_41]]));
// DEFAULT-NEXT:                                                                                                             }
// DEFAULT-NEXT:                                                                                                             write<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_arg1606z00_1028]], read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_v1053z00_1029]]));
// DEFAULT-NEXT:                                                                                                         }
// DEFAULT-NEXT:                                                                                                         {
// DEFAULT-NEXT:                                                                                                             let %[[VALUE_BgL_auxz00_4149:[0-9]+]] BgL_auxz00_4149: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                                                             write<i32>(%[[VALUE_BgL_auxz00_4149]], truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))));
// DEFAULT-NEXT:                                                                                                             write<ptr<@type[[TYPE_scmobj]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_scmobj]]>>, subtract=false, element=ptr<@type[[TYPE_scmobj]]>, overflow=ub>(addr_of<ptr<ptr<@type[[TYPE_scmobj]]>>>(field2(field1(deref(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_v1052z00_1026]]))))), read<i32>(%[[VALUE_BgL_auxz00_4149]]))), read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_arg1606z00_1028]]));
// DEFAULT-NEXT:                                                                                                         }
// DEFAULT-NEXT:                                                                                                     }
// DEFAULT-NEXT:                                                                                                     {
// DEFAULT-NEXT:                                                                                                         let %[[VALUE_BgL_auxz00_4152:[0-9]+]] BgL_auxz00_4152: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                                                         write<i32>(%[[VALUE_BgL_auxz00_4152]], truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))));
// DEFAULT-NEXT:                                                                                                         write<ptr<@type[[TYPE_scmobj]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_scmobj]]>>, subtract=false, element=ptr<@type[[TYPE_scmobj]]>, overflow=ub>(addr_of<ptr<ptr<@type[[TYPE_scmobj]]>>>(field2(field1(deref(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_v1052z00_1026]]))))), read<i32>(%[[VALUE_BgL_auxz00_4152]]))), read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_locz00_43]]));
// DEFAULT-NEXT:                                                                                                     }
// DEFAULT-NEXT:                                                                                                     {
// DEFAULT-NEXT:                                                                                                         let %[[VALUE_BgL_auxz00_4157:[0-9]+]] BgL_auxz00_4157: ptr<@type[[TYPE_scmobj]]> [storage=automatic];
// DEFAULT-NEXT:                                                                                                         let %[[VALUE_BgL_auxz00_4155:[0-9]+]] BgL_auxz00_4155: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                                                         write<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_auxz00_4157]], int_to_ptr<ptr<@type[[TYPE_scmobj]]>, reason=explicit>(or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i64, reason=explicit>(const<i32>(55)), const<i32>(2)), widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                                                                                         write<i32>(%[[VALUE_BgL_auxz00_4155]], truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))));
// DEFAULT-NEXT:                                                                                                         write<ptr<@type[[TYPE_scmobj]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_scmobj]]>>, subtract=false, element=ptr<@type[[TYPE_scmobj]]>, overflow=ub>(addr_of<ptr<ptr<@type[[TYPE_scmobj]]>>>(field2(field1(deref(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_v1052z00_1026]]))))), read<i32>(%[[VALUE_BgL_auxz00_4155]]))), read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_auxz00_4157]]));
// DEFAULT-NEXT:                                                                                                     }
// DEFAULT-NEXT:                                                                                                     return read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_v1052z00_1026]]);
// DEFAULT-NEXT:                                                                                                 }
// DEFAULT-NEXT:                                                                                             else
// DEFAULT-NEXT:                                                                                                 {
// DEFAULT-NEXT:                                                                                                     let %[[VALUE_BgL_v1054z00_1030:[0-9]+]] BgL_v1054z00_1030: ptr<@type[[TYPE_scmobj]]> [storage=automatic];
// DEFAULT-NEXT:                                                                                                     {
// DEFAULT-NEXT:                                                                                                         let %[[VALUE_BgL_auxz00_4160:[0-9]+]] BgL_auxz00_4160: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                                                         write<i32>(%[[VALUE_BgL_auxz00_4160]], truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(3))));
// DEFAULT-NEXT:                                                                                                         write<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_v1054z00_1030]], call<ptr<@type[[TYPE_scmobj]]>, signature=fn(i32) -> ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_create_vector]], read<i32>(%[[VALUE_BgL_auxz00_4160]])));
// DEFAULT-NEXT:                                                                                                         call<ptr<@type[[TYPE_scmobj]]>, signature=fn(i32) -> ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_create_vector]], read<i32>(%[[VALUE_BgL_auxz00_4160]]));
// DEFAULT-NEXT:                                                                                                     }
// DEFAULT-NEXT:                                                                                                     {
// DEFAULT-NEXT:                                                                                                         let %[[VALUE_BgL_arg1608z00_1032:[0-9]+]] BgL_arg1608z00_1032: ptr<@type[[TYPE_scmobj]]> [storage=automatic];
// DEFAULT-NEXT:                                                                                                         write<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_arg1608z00_1032]], call<ptr<@type[[TYPE_scmobj]]>, signature=fn(ptr<@type[[TYPE_scmobj]]>, ptr<@type[[TYPE_scmobj]]>) -> ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_make_pair]], read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_bodyz00_40]]), read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_formalsz00_39]])));
// DEFAULT-NEXT:                                                                                                         call<ptr<@type[[TYPE_scmobj]]>, signature=fn(ptr<@type[[TYPE_scmobj]]>, ptr<@type[[TYPE_scmobj]]>) -> ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_make_pair]], read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_bodyz00_40]]), read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_formalsz00_39]]));
// DEFAULT-NEXT:                                                                                                         {
// DEFAULT-NEXT:                                                                                                             let %[[VALUE_BgL_auxz00_4164:[0-9]+]] BgL_auxz00_4164: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                                                             write<i32>(%[[VALUE_BgL_auxz00_4164]], truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))));
// DEFAULT-NEXT:                                                                                                             write<ptr<@type[[TYPE_scmobj]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_scmobj]]>>, subtract=false, element=ptr<@type[[TYPE_scmobj]]>, overflow=ub>(addr_of<ptr<ptr<@type[[TYPE_scmobj]]>>>(field2(field1(deref(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_v1054z00_1030]]))))), read<i32>(%[[VALUE_BgL_auxz00_4164]]))), read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_arg1608z00_1032]]));
// DEFAULT-NEXT:                                                                                                         }
// DEFAULT-NEXT:                                                                                                     }
// DEFAULT-NEXT:                                                                                                     {
// DEFAULT-NEXT:                                                                                                         let %[[VALUE_BgL_auxz00_4167:[0-9]+]] BgL_auxz00_4167: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                                                         write<i32>(%[[VALUE_BgL_auxz00_4167]], truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))));
// DEFAULT-NEXT:                                                                                                         write<ptr<@type[[TYPE_scmobj]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_scmobj]]>>, subtract=false, element=ptr<@type[[TYPE_scmobj]]>, overflow=ub>(addr_of<ptr<ptr<@type[[TYPE_scmobj]]>>>(field2(field1(deref(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_v1054z00_1030]]))))), read<i32>(%[[VALUE_BgL_auxz00_4167]]))), read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_locz00_43]]));
// DEFAULT-NEXT:                                                                                                     }
// DEFAULT-NEXT:                                                                                                     {
// DEFAULT-NEXT:                                                                                                         let %[[VALUE_BgL_auxz00_4172:[0-9]+]] BgL_auxz00_4172: ptr<@type[[TYPE_scmobj]]> [storage=automatic];
// DEFAULT-NEXT:                                                                                                         let %[[VALUE_BgL_auxz00_4170:[0-9]+]] BgL_auxz00_4170: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                                                         write<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_auxz00_4172]], int_to_ptr<ptr<@type[[TYPE_scmobj]]>, reason=explicit>(or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i64, reason=explicit>(const<i32>(56)), const<i32>(2)), widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                                                                                         write<i32>(%[[VALUE_BgL_auxz00_4170]], truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))));
// DEFAULT-NEXT:                                                                                                         write<ptr<@type[[TYPE_scmobj]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_scmobj]]>>, subtract=false, element=ptr<@type[[TYPE_scmobj]]>, overflow=ub>(addr_of<ptr<ptr<@type[[TYPE_scmobj]]>>>(field2(field1(deref(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_v1054z00_1030]]))))), read<i32>(%[[VALUE_BgL_auxz00_4170]]))), read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_auxz00_4172]]));
// DEFAULT-NEXT:                                                                                                     }
// DEFAULT-NEXT:                                                                                                     return read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_v1054z00_1030]]);
// DEFAULT-NEXT:                                                                                                 }
// DEFAULT-NEXT:                                                                                     }
// DEFAULT-NEXT:                                                                                 else
// DEFAULT-NEXT:                                                                                     {
// DEFAULT-NEXT:                                                                                         let %[[VALUE_BgL_testz00_4175:[0-9]+]] BgL_testz00_4175: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                                         {
// DEFAULT-NEXT:                                                                                             let %[[VALUE_BgL_auxz00_4176:[0-9]+]] BgL_auxz00_4176: ptr<@type[[TYPE_scmobj]]> [storage=automatic];
// DEFAULT-NEXT:                                                                                             {
// DEFAULT-NEXT:                                                                                                 let %[[VALUE_BgL_auxz00_4177:[0-9]+]] BgL_auxz00_4177: ptr<@type[[TYPE_scmobj]]> [storage=automatic];
// DEFAULT-NEXT:                                                                                                 write<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_auxz00_4177]], read<ptr<@type[[TYPE_scmobj]]>>(field1(field0(deref(int_to_ptr<ptr<@type[[TYPE_scmobj]]>, reason=explicit>(sub<i64, overflow=ub>(ptr_to_int<i64, reason=explicit>(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_formalsz00_39]])), widen<i64, reason=usual_arith>(const<i32>(3)))))))));
// DEFAULT-NEXT:                                                                                                 write<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_auxz00_4176]], read<ptr<@type[[TYPE_scmobj]]>>(field0(field0(deref(int_to_ptr<ptr<@type[[TYPE_scmobj]]>, reason=explicit>(sub<i64, overflow=ub>(ptr_to_int<i64, reason=explicit>(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_auxz00_4177]])), widen<i64, reason=usual_arith>(const<i32>(3)))))))));
// DEFAULT-NEXT:                                                                                             }
// DEFAULT-NEXT:                                                                                             write<i32>(%[[VALUE_BgL_testz00_4175]], from_bool<i32, reason=assign>(eq<i64>(and<i64>(ptr_to_int<i64, reason=explicit>(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_auxz00_4176]])), widen<i64, reason=usual_arith>(sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(2)), const<i32>(1)))), widen<i64, reason=usual_arith>(const<i32>(3)))));
// DEFAULT-NEXT:                                                                                         }
// DEFAULT-NEXT:                                                                                         if ne<i32>(read<i32>(%[[VALUE_BgL_testz00_4175]]), const<i32>(0))
// DEFAULT-NEXT:                                                                                             {
// DEFAULT-NEXT:                                                                                                 goto %[[VALUE_BgL_tagzd21971zd2_948]];
// DEFAULT-NEXT:                                                                                             }
// DEFAULT-NEXT:                                                                                         else
// DEFAULT-NEXT:                                                                                             {
// DEFAULT-NEXT:                                                                                                 let %[[VALUE_BgL_testz00_4181:[0-9]+]] BgL_testz00_4181: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                                                 {
// DEFAULT-NEXT:                                                                                                     let %[[VALUE_BgL_auxz00_4182:[0-9]+]] BgL_auxz00_4182: ptr<@type[[TYPE_scmobj]]> [storage=automatic];
// DEFAULT-NEXT:                                                                                                     {
// DEFAULT-NEXT:                                                                                                         let %[[VALUE_BgL_auxz00_4183:[0-9]+]] BgL_auxz00_4183: ptr<@type[[TYPE_scmobj]]> [storage=automatic];
// DEFAULT-NEXT:                                                                                                         {
// DEFAULT-NEXT:                                                                                                             let %[[VALUE_BgL_auxz00_4184:[0-9]+]] BgL_auxz00_4184: ptr<@type[[TYPE_scmobj]]> [storage=automatic];
// DEFAULT-NEXT:                                                                                                             write<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_auxz00_4184]], read<ptr<@type[[TYPE_scmobj]]>>(field1(field0(deref(int_to_ptr<ptr<@type[[TYPE_scmobj]]>, reason=explicit>(sub<i64, overflow=ub>(ptr_to_int<i64, reason=explicit>(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_formalsz00_39]])), widen<i64, reason=usual_arith>(const<i32>(3)))))))));
// DEFAULT-NEXT:                                                                                                             write<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_auxz00_4183]], read<ptr<@type[[TYPE_scmobj]]>>(field1(field0(deref(int_to_ptr<ptr<@type[[TYPE_scmobj]]>, reason=explicit>(sub<i64, overflow=ub>(ptr_to_int<i64, reason=explicit>(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_auxz00_4184]])), widen<i64, reason=usual_arith>(const<i32>(3)))))))));
// DEFAULT-NEXT:                                                                                                         }
// DEFAULT-NEXT:                                                                                                         write<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_auxz00_4182]], read<ptr<@type[[TYPE_scmobj]]>>(field0(field0(deref(int_to_ptr<ptr<@type[[TYPE_scmobj]]>, reason=explicit>(sub<i64, overflow=ub>(ptr_to_int<i64, reason=explicit>(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_auxz00_4183]])), widen<i64, reason=usual_arith>(const<i32>(3)))))))));
// DEFAULT-NEXT:                                                                                                     }
// DEFAULT-NEXT:                                                                                                     write<i32>(%[[VALUE_BgL_testz00_4181]], from_bool<i32, reason=assign>(eq<i64>(and<i64>(ptr_to_int<i64, reason=explicit>(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_auxz00_4182]])), widen<i64, reason=usual_arith>(sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(2)), const<i32>(1)))), widen<i64, reason=usual_arith>(const<i32>(3)))));
// DEFAULT-NEXT:                                                                                                 }
// DEFAULT-NEXT:                                                                                                 if ne<i32>(read<i32>(%[[VALUE_BgL_testz00_4181]]), const<i32>(0))
// DEFAULT-NEXT:                                                                                                     {
// DEFAULT-NEXT:                                                                                                         goto %[[VALUE_BgL_tagzd21971zd2_948]];
// DEFAULT-NEXT:                                                                                                     }
// DEFAULT-NEXT:                                                                                                 else
// DEFAULT-NEXT:                                                                                                     {
// DEFAULT-NEXT:                                                                                                         goto %[[VALUE_BgL_tagzd21971zd2_948]];
// DEFAULT-NEXT:                                                                                                     }
// DEFAULT-NEXT:                                                                                             }
// DEFAULT-NEXT:                                                                                     }
// DEFAULT-NEXT:                                                                             }
// DEFAULT-NEXT:                                                                     }
// DEFAULT-NEXT:                                                                 else
// DEFAULT-NEXT:                                                                     {
// DEFAULT-NEXT:                                                                         let %[[VALUE_BgL_testz00_4189:[0-9]+]] BgL_testz00_4189: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                         {
// DEFAULT-NEXT:                                                                             let %[[VALUE_BgL_auxz00_4190:[0-9]+]] BgL_auxz00_4190: ptr<@type[[TYPE_scmobj]]> [storage=automatic];
// DEFAULT-NEXT:                                                                             write<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_auxz00_4190]], read<ptr<@type[[TYPE_scmobj]]>>(field0(field0(deref(int_to_ptr<ptr<@type[[TYPE_scmobj]]>, reason=explicit>(sub<i64, overflow=ub>(ptr_to_int<i64, reason=explicit>(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_formalsz00_39]])), widen<i64, reason=usual_arith>(const<i32>(3)))))))));
// DEFAULT-NEXT:                                                                             write<i32>(%[[VALUE_BgL_testz00_4189]], from_bool<i32, reason=assign>(eq<i64>(and<i64>(ptr_to_int<i64, reason=explicit>(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_auxz00_4190]])), widen<i64, reason=usual_arith>(sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(2)), const<i32>(1)))), widen<i64, reason=usual_arith>(const<i32>(3)))));
// DEFAULT-NEXT:                                                                         }
// DEFAULT-NEXT:                                                                         if ne<i32>(read<i32>(%[[VALUE_BgL_testz00_4189]]), const<i32>(0))
// DEFAULT-NEXT:                                                                             {
// DEFAULT-NEXT:                                                                                 goto %[[VALUE_BgL_tagzd21971zd2_948]];
// DEFAULT-NEXT:                                                                             }
// DEFAULT-NEXT:                                                                         else
// DEFAULT-NEXT:                                                                             {
// DEFAULT-NEXT:                                                                                 let %[[VALUE_BgL_testz00_4193:[0-9]+]] BgL_testz00_4193: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                                 {
// DEFAULT-NEXT:                                                                                     let %[[VALUE_BgL_auxz00_4194:[0-9]+]] BgL_auxz00_4194: ptr<@type[[TYPE_scmobj]]> [storage=automatic];
// DEFAULT-NEXT:                                                                                     {
// DEFAULT-NEXT:                                                                                         let %[[VALUE_BgL_auxz00_4195:[0-9]+]] BgL_auxz00_4195: ptr<@type[[TYPE_scmobj]]> [storage=automatic];
// DEFAULT-NEXT:                                                                                         write<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_auxz00_4195]], read<ptr<@type[[TYPE_scmobj]]>>(field1(field0(deref(int_to_ptr<ptr<@type[[TYPE_scmobj]]>, reason=explicit>(sub<i64, overflow=ub>(ptr_to_int<i64, reason=explicit>(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_formalsz00_39]])), widen<i64, reason=usual_arith>(const<i32>(3)))))))));
// DEFAULT-NEXT:                                                                                         write<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_auxz00_4194]], read<ptr<@type[[TYPE_scmobj]]>>(field0(field0(deref(int_to_ptr<ptr<@type[[TYPE_scmobj]]>, reason=explicit>(sub<i64, overflow=ub>(ptr_to_int<i64, reason=explicit>(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_auxz00_4195]])), widen<i64, reason=usual_arith>(const<i32>(3)))))))));
// DEFAULT-NEXT:                                                                                     }
// DEFAULT-NEXT:                                                                                     write<i32>(%[[VALUE_BgL_testz00_4193]], from_bool<i32, reason=assign>(eq<i64>(and<i64>(ptr_to_int<i64, reason=explicit>(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_auxz00_4194]])), widen<i64, reason=usual_arith>(sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(2)), const<i32>(1)))), widen<i64, reason=usual_arith>(const<i32>(3)))));
// DEFAULT-NEXT:                                                                                 }
// DEFAULT-NEXT:                                                                                 if ne<i32>(read<i32>(%[[VALUE_BgL_testz00_4193]]), const<i32>(0))
// DEFAULT-NEXT:                                                                                     {
// DEFAULT-NEXT:                                                                                         goto %[[VALUE_BgL_tagzd21971zd2_948]];
// DEFAULT-NEXT:                                                                                     }
// DEFAULT-NEXT:                                                                                 else
// DEFAULT-NEXT:                                                                                     {
// DEFAULT-NEXT:                                                                                         let %[[VALUE_BgL_testz00_4199:[0-9]+]] BgL_testz00_4199: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                                         {
// DEFAULT-NEXT:                                                                                             let %[[VALUE_BgL_auxz00_4200:[0-9]+]] BgL_auxz00_4200: ptr<@type[[TYPE_scmobj]]> [storage=automatic];
// DEFAULT-NEXT:                                                                                             {
// DEFAULT-NEXT:                                                                                                 let %[[VALUE_BgL_auxz00_4201:[0-9]+]] BgL_auxz00_4201: ptr<@type[[TYPE_scmobj]]> [storage=automatic];
// DEFAULT-NEXT:                                                                                                 {
// DEFAULT-NEXT:                                                                                                     let %[[VALUE_BgL_auxz00_4202:[0-9]+]] BgL_auxz00_4202: ptr<@type[[TYPE_scmobj]]> [storage=automatic];
// DEFAULT-NEXT:                                                                                                     write<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_auxz00_4202]], read<ptr<@type[[TYPE_scmobj]]>>(field1(field0(deref(int_to_ptr<ptr<@type[[TYPE_scmobj]]>, reason=explicit>(sub<i64, overflow=ub>(ptr_to_int<i64, reason=explicit>(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_formalsz00_39]])), widen<i64, reason=usual_arith>(const<i32>(3)))))))));
// DEFAULT-NEXT:                                                                                                     write<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_auxz00_4201]], read<ptr<@type[[TYPE_scmobj]]>>(field1(field0(deref(int_to_ptr<ptr<@type[[TYPE_scmobj]]>, reason=explicit>(sub<i64, overflow=ub>(ptr_to_int<i64, reason=explicit>(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_auxz00_4202]])), widen<i64, reason=usual_arith>(const<i32>(3)))))))));
// DEFAULT-NEXT:                                                                                                 }
// DEFAULT-NEXT:                                                                                                 write<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_auxz00_4200]], read<ptr<@type[[TYPE_scmobj]]>>(field0(field0(deref(int_to_ptr<ptr<@type[[TYPE_scmobj]]>, reason=explicit>(sub<i64, overflow=ub>(ptr_to_int<i64, reason=explicit>(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_auxz00_4201]])), widen<i64, reason=usual_arith>(const<i32>(3)))))))));
// DEFAULT-NEXT:                                                                                             }
// DEFAULT-NEXT:                                                                                             write<i32>(%[[VALUE_BgL_testz00_4199]], from_bool<i32, reason=assign>(eq<i64>(and<i64>(ptr_to_int<i64, reason=explicit>(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_auxz00_4200]])), widen<i64, reason=usual_arith>(sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(2)), const<i32>(1)))), widen<i64, reason=usual_arith>(const<i32>(3)))));
// DEFAULT-NEXT:                                                                                         }
// DEFAULT-NEXT:                                                                                         if ne<i32>(read<i32>(%[[VALUE_BgL_testz00_4199]]), const<i32>(0))
// DEFAULT-NEXT:                                                                                             {
// DEFAULT-NEXT:                                                                                                 goto %[[VALUE_BgL_tagzd21971zd2_948]];
// DEFAULT-NEXT:                                                                                             }
// DEFAULT-NEXT:                                                                                         else
// DEFAULT-NEXT:                                                                                             {
// DEFAULT-NEXT:                                                                                                 if ne<ptr<@type[[TYPE_scmobj]]>>(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_namedzf3zf3_42]]), int_to_ptr<ptr<@type[[TYPE_scmobj]]>, reason=explicit>(or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i64, reason=explicit>(const<i32>(1)), const<i32>(2)), widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:                                                                                                     {
// DEFAULT-NEXT:                                                                                                         let %[[VALUE_BgL_v1050z00_1022:[0-9]+]] BgL_v1050z00_1022: ptr<@type[[TYPE_scmobj]]> [storage=automatic];
// DEFAULT-NEXT:                                                                                                         {
// DEFAULT-NEXT:                                                                                                             let %[[VALUE_BgL_auxz00_4209:[0-9]+]] BgL_auxz00_4209: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                                                             write<i32>(%[[VALUE_BgL_auxz00_4209]], truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(3))));
// DEFAULT-NEXT:                                                                                                             write<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_v1050z00_1022]], call<ptr<@type[[TYPE_scmobj]]>, signature=fn(i32) -> ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_create_vector]], read<i32>(%[[VALUE_BgL_auxz00_4209]])));
// DEFAULT-NEXT:                                                                                                             call<ptr<@type[[TYPE_scmobj]]>, signature=fn(i32) -> ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_create_vector]], read<i32>(%[[VALUE_BgL_auxz00_4209]]));
// DEFAULT-NEXT:                                                                                                         }
// DEFAULT-NEXT:                                                                                                         {
// DEFAULT-NEXT:                                                                                                             let %[[VALUE_BgL_arg1604z00_1024:[0-9]+]] BgL_arg1604z00_1024: ptr<@type[[TYPE_scmobj]]> [storage=automatic];
// DEFAULT-NEXT:                                                                                                             write<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_arg1604z00_1024]], call<ptr<@type[[TYPE_scmobj]]>, signature=fn(ptr<@type[[TYPE_scmobj]]>, ptr<@type[[TYPE_scmobj]]>) -> ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_make_pair]], read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_wherez00_41]]), read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_bodyz00_40]])));
// DEFAULT-NEXT:                                                                                                             call<ptr<@type[[TYPE_scmobj]]>, signature=fn(ptr<@type[[TYPE_scmobj]]>, ptr<@type[[TYPE_scmobj]]>) -> ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_make_pair]], read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_wherez00_41]]), read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_bodyz00_40]]));
// DEFAULT-NEXT:                                                                                                             {
// DEFAULT-NEXT:                                                                                                                 let %[[VALUE_BgL_auxz00_4213:[0-9]+]] BgL_auxz00_4213: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                                                                 write<i32>(%[[VALUE_BgL_auxz00_4213]], truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))));
// DEFAULT-NEXT:                                                                                                                 write<ptr<@type[[TYPE_scmobj]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_scmobj]]>>, subtract=false, element=ptr<@type[[TYPE_scmobj]]>, overflow=ub>(addr_of<ptr<ptr<@type[[TYPE_scmobj]]>>>(field2(field1(deref(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_v1050z00_1022]]))))), read<i32>(%[[VALUE_BgL_auxz00_4213]]))), read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_arg1604z00_1024]]));
// DEFAULT-NEXT:                                                                                                             }
// DEFAULT-NEXT:                                                                                                         }
// DEFAULT-NEXT:                                                                                                         {
// DEFAULT-NEXT:                                                                                                             let %[[VALUE_BgL_auxz00_4216:[0-9]+]] BgL_auxz00_4216: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                                                             write<i32>(%[[VALUE_BgL_auxz00_4216]], truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))));
// DEFAULT-NEXT:                                                                                                             write<ptr<@type[[TYPE_scmobj]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_scmobj]]>>, subtract=false, element=ptr<@type[[TYPE_scmobj]]>, overflow=ub>(addr_of<ptr<ptr<@type[[TYPE_scmobj]]>>>(field2(field1(deref(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_v1050z00_1022]]))))), read<i32>(%[[VALUE_BgL_auxz00_4216]]))), read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_locz00_43]]));
// DEFAULT-NEXT:                                                                                                         }
// DEFAULT-NEXT:                                                                                                         {
// DEFAULT-NEXT:                                                                                                             let %[[VALUE_BgL_auxz00_4221:[0-9]+]] BgL_auxz00_4221: ptr<@type[[TYPE_scmobj]]> [storage=automatic];
// DEFAULT-NEXT:                                                                                                             let %[[VALUE_BgL_auxz00_4219:[0-9]+]] BgL_auxz00_4219: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                                                             write<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_auxz00_4221]], int_to_ptr<ptr<@type[[TYPE_scmobj]]>, reason=explicit>(or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i64, reason=explicit>(const<i32>(50)), const<i32>(2)), widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                                                                                             write<i32>(%[[VALUE_BgL_auxz00_4219]], truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))));
// DEFAULT-NEXT:                                                                                                             write<ptr<@type[[TYPE_scmobj]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_scmobj]]>>, subtract=false, element=ptr<@type[[TYPE_scmobj]]>, overflow=ub>(addr_of<ptr<ptr<@type[[TYPE_scmobj]]>>>(field2(field1(deref(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_v1050z00_1022]]))))), read<i32>(%[[VALUE_BgL_auxz00_4219]]))), read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_auxz00_4221]]));
// DEFAULT-NEXT:                                                                                                         }
// DEFAULT-NEXT:                                                                                                         return read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_v1050z00_1022]]);
// DEFAULT-NEXT:                                                                                                     }
// DEFAULT-NEXT:                                                                                                 else
// DEFAULT-NEXT:                                                                                                     {
// DEFAULT-NEXT:                                                                                                         let %[[VALUE_BgL_v1051z00_1025:[0-9]+]] BgL_v1051z00_1025: ptr<@type[[TYPE_scmobj]]> [storage=automatic];
// DEFAULT-NEXT:                                                                                                         {
// DEFAULT-NEXT:                                                                                                             let %[[VALUE_BgL_auxz00_4224:[0-9]+]] BgL_auxz00_4224: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                                                             write<i32>(%[[VALUE_BgL_auxz00_4224]], truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(3))));
// DEFAULT-NEXT:                                                                                                             write<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_v1051z00_1025]], call<ptr<@type[[TYPE_scmobj]]>, signature=fn(i32) -> ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_create_vector]], read<i32>(%[[VALUE_BgL_auxz00_4224]])));
// DEFAULT-NEXT:                                                                                                             call<ptr<@type[[TYPE_scmobj]]>, signature=fn(i32) -> ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_create_vector]], read<i32>(%[[VALUE_BgL_auxz00_4224]]));
// DEFAULT-NEXT:                                                                                                         }
// DEFAULT-NEXT:                                                                                                         {
// DEFAULT-NEXT:                                                                                                             let %[[VALUE_BgL_auxz00_4227:[0-9]+]] BgL_auxz00_4227: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                                                             write<i32>(%[[VALUE_BgL_auxz00_4227]], truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))));
// DEFAULT-NEXT:                                                                                                             write<ptr<@type[[TYPE_scmobj]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_scmobj]]>>, subtract=false, element=ptr<@type[[TYPE_scmobj]]>, overflow=ub>(addr_of<ptr<ptr<@type[[TYPE_scmobj]]>>>(field2(field1(deref(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_v1051z00_1025]]))))), read<i32>(%[[VALUE_BgL_auxz00_4227]]))), read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_bodyz00_40]]));
// DEFAULT-NEXT:                                                                                                         }
// DEFAULT-NEXT:                                                                                                         {
// DEFAULT-NEXT:                                                                                                             let %[[VALUE_BgL_auxz00_4230:[0-9]+]] BgL_auxz00_4230: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                                                             write<i32>(%[[VALUE_BgL_auxz00_4230]], truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))));
// DEFAULT-NEXT:                                                                                                             write<ptr<@type[[TYPE_scmobj]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_scmobj]]>>, subtract=false, element=ptr<@type[[TYPE_scmobj]]>, overflow=ub>(addr_of<ptr<ptr<@type[[TYPE_scmobj]]>>>(field2(field1(deref(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_v1051z00_1025]]))))), read<i32>(%[[VALUE_BgL_auxz00_4230]]))), read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_locz00_43]]));
// DEFAULT-NEXT:                                                                                                         }
// DEFAULT-NEXT:                                                                                                         {
// DEFAULT-NEXT:                                                                                                             let %[[VALUE_BgL_auxz00_4235:[0-9]+]] BgL_auxz00_4235: ptr<@type[[TYPE_scmobj]]> [storage=automatic];
// DEFAULT-NEXT:                                                                                                             let %[[VALUE_BgL_auxz00_4233:[0-9]+]] BgL_auxz00_4233: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                                                             write<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_auxz00_4235]], int_to_ptr<ptr<@type[[TYPE_scmobj]]>, reason=explicit>(or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i64, reason=explicit>(const<i32>(54)), const<i32>(2)), widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                                                                                             write<i32>(%[[VALUE_BgL_auxz00_4233]], truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))));
// DEFAULT-NEXT:                                                                                                             write<ptr<@type[[TYPE_scmobj]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_scmobj]]>>, subtract=false, element=ptr<@type[[TYPE_scmobj]]>, overflow=ub>(addr_of<ptr<ptr<@type[[TYPE_scmobj]]>>>(field2(field1(deref(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_v1051z00_1025]]))))), read<i32>(%[[VALUE_BgL_auxz00_4233]]))), read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_auxz00_4235]]));
// DEFAULT-NEXT:                                                                                                         }
// DEFAULT-NEXT:                                                                                                         return read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_v1051z00_1025]]);
// DEFAULT-NEXT:                                                                                                     }
// DEFAULT-NEXT:                                                                                             }
// DEFAULT-NEXT:                                                                                     }
// DEFAULT-NEXT:                                                                             }
// DEFAULT-NEXT:                                                                     }
// DEFAULT-NEXT:                                                             }
// DEFAULT-NEXT:                                                     }
// DEFAULT-NEXT:                                                 else
// DEFAULT-NEXT:                                                     {
// DEFAULT-NEXT:                                                         let %[[VALUE_BgL_testz00_4238:[0-9]+]] BgL_testz00_4238: i32 [storage=automatic];
// DEFAULT-NEXT:                                                         {
// DEFAULT-NEXT:                                                             let %[[VALUE_BgL_auxz00_4239:[0-9]+]] BgL_auxz00_4239: ptr<@type[[TYPE_scmobj]]> [storage=automatic];
// DEFAULT-NEXT:                                                             write<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_auxz00_4239]], read<ptr<@type[[TYPE_scmobj]]>>(field0(field0(deref(int_to_ptr<ptr<@type[[TYPE_scmobj]]>, reason=explicit>(sub<i64, overflow=ub>(ptr_to_int<i64, reason=explicit>(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_formalsz00_39]])), widen<i64, reason=usual_arith>(const<i32>(3)))))))));
// DEFAULT-NEXT:                                                             write<i32>(%[[VALUE_BgL_testz00_4238]], from_bool<i32, reason=assign>(eq<i64>(and<i64>(ptr_to_int<i64, reason=explicit>(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_auxz00_4239]])), widen<i64, reason=usual_arith>(sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(2)), const<i32>(1)))), widen<i64, reason=usual_arith>(const<i32>(3)))));
// DEFAULT-NEXT:                                                         }
// DEFAULT-NEXT:                                                         if ne<i32>(read<i32>(%[[VALUE_BgL_testz00_4238]]), const<i32>(0))
// DEFAULT-NEXT:                                                             {
// DEFAULT-NEXT:                                                                 goto %[[VALUE_BgL_tagzd21971zd2_948]];
// DEFAULT-NEXT:                                                             }
// DEFAULT-NEXT:                                                         else
// DEFAULT-NEXT:                                                             {
// DEFAULT-NEXT:                                                                 let %[[VALUE_BgL_testz00_4242:[0-9]+]] BgL_testz00_4242: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                 {
// DEFAULT-NEXT:                                                                     let %[[VALUE_BgL_auxz00_4243:[0-9]+]] BgL_auxz00_4243: ptr<@type[[TYPE_scmobj]]> [storage=automatic];
// DEFAULT-NEXT:                                                                     write<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_auxz00_4243]], read<ptr<@type[[TYPE_scmobj]]>>(field0(field0(deref(int_to_ptr<ptr<@type[[TYPE_scmobj]]>, reason=explicit>(sub<i64, overflow=ub>(ptr_to_int<i64, reason=explicit>(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_cdrzd21979zd2_953]])), widen<i64, reason=usual_arith>(const<i32>(3)))))))));
// DEFAULT-NEXT:                                                                     write<i32>(%[[VALUE_BgL_testz00_4242]], from_bool<i32, reason=assign>(eq<i64>(and<i64>(ptr_to_int<i64, reason=explicit>(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_auxz00_4243]])), widen<i64, reason=usual_arith>(sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(2)), const<i32>(1)))), widen<i64, reason=usual_arith>(const<i32>(3)))));
// DEFAULT-NEXT:                                                                 }
// DEFAULT-NEXT:                                                                 if ne<i32>(read<i32>(%[[VALUE_BgL_testz00_4242]]), const<i32>(0))
// DEFAULT-NEXT:                                                                     {
// DEFAULT-NEXT:                                                                         goto %[[VALUE_BgL_tagzd21971zd2_948]];
// DEFAULT-NEXT:                                                                     }
// DEFAULT-NEXT:                                                                 else
// DEFAULT-NEXT:                                                                     {
// DEFAULT-NEXT:                                                                         if ne<ptr<@type[[TYPE_scmobj]]>>(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_namedzf3zf3_42]]), int_to_ptr<ptr<@type[[TYPE_scmobj]]>, reason=explicit>(or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i64, reason=explicit>(const<i32>(1)), const<i32>(2)), widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:                                                                             {
// DEFAULT-NEXT:                                                                                 let %[[VALUE_BgL_v1048z00_1018:[0-9]+]] BgL_v1048z00_1018: ptr<@type[[TYPE_scmobj]]> [storage=automatic];
// DEFAULT-NEXT:                                                                                 {
// DEFAULT-NEXT:                                                                                     let %[[VALUE_BgL_auxz00_4248:[0-9]+]] BgL_auxz00_4248: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                                     write<i32>(%[[VALUE_BgL_auxz00_4248]], truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(3))));
// DEFAULT-NEXT:                                                                                     write<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_v1048z00_1018]], call<ptr<@type[[TYPE_scmobj]]>, signature=fn(i32) -> ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_create_vector]], read<i32>(%[[VALUE_BgL_auxz00_4248]])));
// DEFAULT-NEXT:                                                                                     call<ptr<@type[[TYPE_scmobj]]>, signature=fn(i32) -> ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_create_vector]], read<i32>(%[[VALUE_BgL_auxz00_4248]]));
// DEFAULT-NEXT:                                                                                 }
// DEFAULT-NEXT:                                                                                 {
// DEFAULT-NEXT:                                                                                     let %[[VALUE_BgL_arg1602z00_1020:[0-9]+]] BgL_arg1602z00_1020: ptr<@type[[TYPE_scmobj]]> [storage=automatic];
// DEFAULT-NEXT:                                                                                     write<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_arg1602z00_1020]], call<ptr<@type[[TYPE_scmobj]]>, signature=fn(ptr<@type[[TYPE_scmobj]]>, ptr<@type[[TYPE_scmobj]]>) -> ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_make_pair]], read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_wherez00_41]]), read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_bodyz00_40]])));
// DEFAULT-NEXT:                                                                                     call<ptr<@type[[TYPE_scmobj]]>, signature=fn(ptr<@type[[TYPE_scmobj]]>, ptr<@type[[TYPE_scmobj]]>) -> ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_make_pair]], read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_wherez00_41]]), read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_bodyz00_40]]));
// DEFAULT-NEXT:                                                                                     {
// DEFAULT-NEXT:                                                                                         let %[[VALUE_BgL_auxz00_4252:[0-9]+]] BgL_auxz00_4252: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                                         write<i32>(%[[VALUE_BgL_auxz00_4252]], truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))));
// DEFAULT-NEXT:                                                                                         write<ptr<@type[[TYPE_scmobj]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_scmobj]]>>, subtract=false, element=ptr<@type[[TYPE_scmobj]]>, overflow=ub>(addr_of<ptr<ptr<@type[[TYPE_scmobj]]>>>(field2(field1(deref(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_v1048z00_1018]]))))), read<i32>(%[[VALUE_BgL_auxz00_4252]]))), read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_arg1602z00_1020]]));
// DEFAULT-NEXT:                                                                                     }
// DEFAULT-NEXT:                                                                                 }
// DEFAULT-NEXT:                                                                                 {
// DEFAULT-NEXT:                                                                                     let %[[VALUE_BgL_auxz00_4255:[0-9]+]] BgL_auxz00_4255: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                                     write<i32>(%[[VALUE_BgL_auxz00_4255]], truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))));
// DEFAULT-NEXT:                                                                                     write<ptr<@type[[TYPE_scmobj]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_scmobj]]>>, subtract=false, element=ptr<@type[[TYPE_scmobj]]>, overflow=ub>(addr_of<ptr<ptr<@type[[TYPE_scmobj]]>>>(field2(field1(deref(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_v1048z00_1018]]))))), read<i32>(%[[VALUE_BgL_auxz00_4255]]))), read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_locz00_43]]));
// DEFAULT-NEXT:                                                                                 }
// DEFAULT-NEXT:                                                                                 {
// DEFAULT-NEXT:                                                                                     let %[[VALUE_BgL_auxz00_4260:[0-9]+]] BgL_auxz00_4260: ptr<@type[[TYPE_scmobj]]> [storage=automatic];
// DEFAULT-NEXT:                                                                                     let %[[VALUE_BgL_auxz00_4258:[0-9]+]] BgL_auxz00_4258: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                                     write<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_auxz00_4260]], int_to_ptr<ptr<@type[[TYPE_scmobj]]>, reason=explicit>(or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i64, reason=explicit>(const<i32>(49)), const<i32>(2)), widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                                                                     write<i32>(%[[VALUE_BgL_auxz00_4258]], truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))));
// DEFAULT-NEXT:                                                                                     write<ptr<@type[[TYPE_scmobj]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_scmobj]]>>, subtract=false, element=ptr<@type[[TYPE_scmobj]]>, overflow=ub>(addr_of<ptr<ptr<@type[[TYPE_scmobj]]>>>(field2(field1(deref(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_v1048z00_1018]]))))), read<i32>(%[[VALUE_BgL_auxz00_4258]]))), read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_auxz00_4260]]));
// DEFAULT-NEXT:                                                                                 }
// DEFAULT-NEXT:                                                                                 return read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_v1048z00_1018]]);
// DEFAULT-NEXT:                                                                             }
// DEFAULT-NEXT:                                                                         else
// DEFAULT-NEXT:                                                                             {
// DEFAULT-NEXT:                                                                                 let %[[VALUE_BgL_v1049z00_1021:[0-9]+]] BgL_v1049z00_1021: ptr<@type[[TYPE_scmobj]]> [storage=automatic];
// DEFAULT-NEXT:                                                                                 {
// DEFAULT-NEXT:                                                                                     let %[[VALUE_BgL_auxz00_4263:[0-9]+]] BgL_auxz00_4263: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                                     write<i32>(%[[VALUE_BgL_auxz00_4263]], truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(3))));
// DEFAULT-NEXT:                                                                                     write<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_v1049z00_1021]], call<ptr<@type[[TYPE_scmobj]]>, signature=fn(i32) -> ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_create_vector]], read<i32>(%[[VALUE_BgL_auxz00_4263]])));
// DEFAULT-NEXT:                                                                                     call<ptr<@type[[TYPE_scmobj]]>, signature=fn(i32) -> ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_create_vector]], read<i32>(%[[VALUE_BgL_auxz00_4263]]));
// DEFAULT-NEXT:                                                                                 }
// DEFAULT-NEXT:                                                                                 {
// DEFAULT-NEXT:                                                                                     let %[[VALUE_BgL_auxz00_4266:[0-9]+]] BgL_auxz00_4266: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                                     write<i32>(%[[VALUE_BgL_auxz00_4266]], truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))));
// DEFAULT-NEXT:                                                                                     write<ptr<@type[[TYPE_scmobj]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_scmobj]]>>, subtract=false, element=ptr<@type[[TYPE_scmobj]]>, overflow=ub>(addr_of<ptr<ptr<@type[[TYPE_scmobj]]>>>(field2(field1(deref(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_v1049z00_1021]]))))), read<i32>(%[[VALUE_BgL_auxz00_4266]]))), read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_bodyz00_40]]));
// DEFAULT-NEXT:                                                                                 }
// DEFAULT-NEXT:                                                                                 {
// DEFAULT-NEXT:                                                                                     let %[[VALUE_BgL_auxz00_4269:[0-9]+]] BgL_auxz00_4269: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                                     write<i32>(%[[VALUE_BgL_auxz00_4269]], truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))));
// DEFAULT-NEXT:                                                                                     write<ptr<@type[[TYPE_scmobj]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_scmobj]]>>, subtract=false, element=ptr<@type[[TYPE_scmobj]]>, overflow=ub>(addr_of<ptr<ptr<@type[[TYPE_scmobj]]>>>(field2(field1(deref(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_v1049z00_1021]]))))), read<i32>(%[[VALUE_BgL_auxz00_4269]]))), read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_locz00_43]]));
// DEFAULT-NEXT:                                                                                 }
// DEFAULT-NEXT:                                                                                 {
// DEFAULT-NEXT:                                                                                     let %[[VALUE_BgL_auxz00_4274:[0-9]+]] BgL_auxz00_4274: ptr<@type[[TYPE_scmobj]]> [storage=automatic];
// DEFAULT-NEXT:                                                                                     let %[[VALUE_BgL_auxz00_4272:[0-9]+]] BgL_auxz00_4272: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                                     write<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_auxz00_4274]], int_to_ptr<ptr<@type[[TYPE_scmobj]]>, reason=explicit>(or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i64, reason=explicit>(const<i32>(53)), const<i32>(2)), widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                                                                     write<i32>(%[[VALUE_BgL_auxz00_4272]], truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))));
// DEFAULT-NEXT:                                                                                     write<ptr<@type[[TYPE_scmobj]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_scmobj]]>>, subtract=false, element=ptr<@type[[TYPE_scmobj]]>, overflow=ub>(addr_of<ptr<ptr<@type[[TYPE_scmobj]]>>>(field2(field1(deref(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_v1049z00_1021]]))))), read<i32>(%[[VALUE_BgL_auxz00_4272]]))), read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_auxz00_4274]]));
// DEFAULT-NEXT:                                                                                 }
// DEFAULT-NEXT:                                                                                 return read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_v1049z00_1021]]);
// DEFAULT-NEXT:                                                                             }
// DEFAULT-NEXT:                                                                     }
// DEFAULT-NEXT:                                                             }
// DEFAULT-NEXT:                                                     }
// DEFAULT-NEXT:                                             }
// DEFAULT-NEXT:                                     }
// DEFAULT-NEXT:                                 else
// DEFAULT-NEXT:                                     {
// DEFAULT-NEXT:                                         let %[[VALUE_BgL_testz00_4277:[0-9]+]] BgL_testz00_4277: i32 [storage=automatic];
// DEFAULT-NEXT:                                         {
// DEFAULT-NEXT:                                             let %[[VALUE_BgL_auxz00_4278:[0-9]+]] BgL_auxz00_4278: ptr<@type[[TYPE_scmobj]]> [storage=automatic];
// DEFAULT-NEXT:                                             write<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_auxz00_4278]], read<ptr<@type[[TYPE_scmobj]]>>(field0(field0(deref(int_to_ptr<ptr<@type[[TYPE_scmobj]]>, reason=explicit>(sub<i64, overflow=ub>(ptr_to_int<i64, reason=explicit>(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_formalsz00_39]])), widen<i64, reason=usual_arith>(const<i32>(3)))))))));
// DEFAULT-NEXT:                                             write<i32>(%[[VALUE_BgL_testz00_4277]], from_bool<i32, reason=assign>(eq<i64>(and<i64>(ptr_to_int<i64, reason=explicit>(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_auxz00_4278]])), widen<i64, reason=usual_arith>(sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(2)), const<i32>(1)))), widen<i64, reason=usual_arith>(const<i32>(3)))));
// DEFAULT-NEXT:                                         }
// DEFAULT-NEXT:                                         if ne<i32>(read<i32>(%[[VALUE_BgL_testz00_4277]]), const<i32>(0))
// DEFAULT-NEXT:                                             {
// DEFAULT-NEXT:                                                 goto %[[VALUE_BgL_tagzd21971zd2_948]];
// DEFAULT-NEXT:                                             }
// DEFAULT-NEXT:                                         else
// DEFAULT-NEXT:                                             {
// DEFAULT-NEXT:                                                 if ne<ptr<@type[[TYPE_scmobj]]>>(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_namedzf3zf3_42]]), int_to_ptr<ptr<@type[[TYPE_scmobj]]>, reason=explicit>(or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i64, reason=explicit>(const<i32>(1)), const<i32>(2)), widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:                                                     {
// DEFAULT-NEXT:                                                         let %[[VALUE_BgL_v1046z00_1014:[0-9]+]] BgL_v1046z00_1014: ptr<@type[[TYPE_scmobj]]> [storage=automatic];
// DEFAULT-NEXT:                                                         {
// DEFAULT-NEXT:                                                             let %[[VALUE_BgL_auxz00_4283:[0-9]+]] BgL_auxz00_4283: i32 [storage=automatic];
// DEFAULT-NEXT:                                                             write<i32>(%[[VALUE_BgL_auxz00_4283]], truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(3))));
// DEFAULT-NEXT:                                                             write<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_v1046z00_1014]], call<ptr<@type[[TYPE_scmobj]]>, signature=fn(i32) -> ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_create_vector]], read<i32>(%[[VALUE_BgL_auxz00_4283]])));
// DEFAULT-NEXT:                                                             call<ptr<@type[[TYPE_scmobj]]>, signature=fn(i32) -> ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_create_vector]], read<i32>(%[[VALUE_BgL_auxz00_4283]]));
// DEFAULT-NEXT:                                                         }
// DEFAULT-NEXT:                                                         {
// DEFAULT-NEXT:                                                             let %[[VALUE_BgL_arg1600z00_1016:[0-9]+]] BgL_arg1600z00_1016: ptr<@type[[TYPE_scmobj]]> [storage=automatic];
// DEFAULT-NEXT:                                                             write<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_arg1600z00_1016]], call<ptr<@type[[TYPE_scmobj]]>, signature=fn(ptr<@type[[TYPE_scmobj]]>, ptr<@type[[TYPE_scmobj]]>) -> ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_make_pair]], read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_wherez00_41]]), read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_bodyz00_40]])));
// DEFAULT-NEXT:                                                             call<ptr<@type[[TYPE_scmobj]]>, signature=fn(ptr<@type[[TYPE_scmobj]]>, ptr<@type[[TYPE_scmobj]]>) -> ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_make_pair]], read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_wherez00_41]]), read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_bodyz00_40]]));
// DEFAULT-NEXT:                                                             {
// DEFAULT-NEXT:                                                                 let %[[VALUE_BgL_auxz00_4287:[0-9]+]] BgL_auxz00_4287: i32 [storage=automatic];
// DEFAULT-NEXT:                                                                 write<i32>(%[[VALUE_BgL_auxz00_4287]], truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))));
// DEFAULT-NEXT:                                                                 write<ptr<@type[[TYPE_scmobj]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_scmobj]]>>, subtract=false, element=ptr<@type[[TYPE_scmobj]]>, overflow=ub>(addr_of<ptr<ptr<@type[[TYPE_scmobj]]>>>(field2(field1(deref(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_v1046z00_1014]]))))), read<i32>(%[[VALUE_BgL_auxz00_4287]]))), read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_arg1600z00_1016]]));
// DEFAULT-NEXT:                                                             }
// DEFAULT-NEXT:                                                         }
// DEFAULT-NEXT:                                                         {
// DEFAULT-NEXT:                                                             let %[[VALUE_BgL_auxz00_4290:[0-9]+]] BgL_auxz00_4290: i32 [storage=automatic];
// DEFAULT-NEXT:                                                             write<i32>(%[[VALUE_BgL_auxz00_4290]], truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))));
// DEFAULT-NEXT:                                                             write<ptr<@type[[TYPE_scmobj]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_scmobj]]>>, subtract=false, element=ptr<@type[[TYPE_scmobj]]>, overflow=ub>(addr_of<ptr<ptr<@type[[TYPE_scmobj]]>>>(field2(field1(deref(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_v1046z00_1014]]))))), read<i32>(%[[VALUE_BgL_auxz00_4290]]))), read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_locz00_43]]));
// DEFAULT-NEXT:                                                         }
// DEFAULT-NEXT:                                                         {
// DEFAULT-NEXT:                                                             let %[[VALUE_BgL_auxz00_4295:[0-9]+]] BgL_auxz00_4295: ptr<@type[[TYPE_scmobj]]> [storage=automatic];
// DEFAULT-NEXT:                                                             let %[[VALUE_BgL_auxz00_4293:[0-9]+]] BgL_auxz00_4293: i32 [storage=automatic];
// DEFAULT-NEXT:                                                             write<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_auxz00_4295]], int_to_ptr<ptr<@type[[TYPE_scmobj]]>, reason=explicit>(or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i64, reason=explicit>(const<i32>(48)), const<i32>(2)), widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                                             write<i32>(%[[VALUE_BgL_auxz00_4293]], truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))));
// DEFAULT-NEXT:                                                             write<ptr<@type[[TYPE_scmobj]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_scmobj]]>>, subtract=false, element=ptr<@type[[TYPE_scmobj]]>, overflow=ub>(addr_of<ptr<ptr<@type[[TYPE_scmobj]]>>>(field2(field1(deref(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_v1046z00_1014]]))))), read<i32>(%[[VALUE_BgL_auxz00_4293]]))), read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_auxz00_4295]]));
// DEFAULT-NEXT:                                                         }
// DEFAULT-NEXT:                                                         return read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_v1046z00_1014]]);
// DEFAULT-NEXT:                                                     }
// DEFAULT-NEXT:                                                 else
// DEFAULT-NEXT:                                                     {
// DEFAULT-NEXT:                                                         let %[[VALUE_BgL_v1047z00_1017:[0-9]+]] BgL_v1047z00_1017: ptr<@type[[TYPE_scmobj]]> [storage=automatic];
// DEFAULT-NEXT:                                                         {
// DEFAULT-NEXT:                                                             let %[[VALUE_BgL_auxz00_4298:[0-9]+]] BgL_auxz00_4298: i32 [storage=automatic];
// DEFAULT-NEXT:                                                             write<i32>(%[[VALUE_BgL_auxz00_4298]], truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(3))));
// DEFAULT-NEXT:                                                             write<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_v1047z00_1017]], call<ptr<@type[[TYPE_scmobj]]>, signature=fn(i32) -> ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_create_vector]], read<i32>(%[[VALUE_BgL_auxz00_4298]])));
// DEFAULT-NEXT:                                                             call<ptr<@type[[TYPE_scmobj]]>, signature=fn(i32) -> ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_create_vector]], read<i32>(%[[VALUE_BgL_auxz00_4298]]));
// DEFAULT-NEXT:                                                         }
// DEFAULT-NEXT:                                                         {
// DEFAULT-NEXT:                                                             let %[[VALUE_BgL_auxz00_4301:[0-9]+]] BgL_auxz00_4301: i32 [storage=automatic];
// DEFAULT-NEXT:                                                             write<i32>(%[[VALUE_BgL_auxz00_4301]], truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))));
// DEFAULT-NEXT:                                                             write<ptr<@type[[TYPE_scmobj]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_scmobj]]>>, subtract=false, element=ptr<@type[[TYPE_scmobj]]>, overflow=ub>(addr_of<ptr<ptr<@type[[TYPE_scmobj]]>>>(field2(field1(deref(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_v1047z00_1017]]))))), read<i32>(%[[VALUE_BgL_auxz00_4301]]))), read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_bodyz00_40]]));
// DEFAULT-NEXT:                                                         }
// DEFAULT-NEXT:                                                         {
// DEFAULT-NEXT:                                                             let %[[VALUE_BgL_auxz00_4304:[0-9]+]] BgL_auxz00_4304: i32 [storage=automatic];
// DEFAULT-NEXT:                                                             write<i32>(%[[VALUE_BgL_auxz00_4304]], truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))));
// DEFAULT-NEXT:                                                             write<ptr<@type[[TYPE_scmobj]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_scmobj]]>>, subtract=false, element=ptr<@type[[TYPE_scmobj]]>, overflow=ub>(addr_of<ptr<ptr<@type[[TYPE_scmobj]]>>>(field2(field1(deref(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_v1047z00_1017]]))))), read<i32>(%[[VALUE_BgL_auxz00_4304]]))), read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_locz00_43]]));
// DEFAULT-NEXT:                                                         }
// DEFAULT-NEXT:                                                         {
// DEFAULT-NEXT:                                                             let %[[VALUE_BgL_auxz00_4309:[0-9]+]] BgL_auxz00_4309: ptr<@type[[TYPE_scmobj]]> [storage=automatic];
// DEFAULT-NEXT:                                                             let %[[VALUE_BgL_auxz00_4307:[0-9]+]] BgL_auxz00_4307: i32 [storage=automatic];
// DEFAULT-NEXT:                                                             write<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_auxz00_4309]], int_to_ptr<ptr<@type[[TYPE_scmobj]]>, reason=explicit>(or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i64, reason=explicit>(const<i32>(52)), const<i32>(2)), widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                                             write<i32>(%[[VALUE_BgL_auxz00_4307]], truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))));
// DEFAULT-NEXT:                                                             write<ptr<@type[[TYPE_scmobj]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_scmobj]]>>, subtract=false, element=ptr<@type[[TYPE_scmobj]]>, overflow=ub>(addr_of<ptr<ptr<@type[[TYPE_scmobj]]>>>(field2(field1(deref(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_v1047z00_1017]]))))), read<i32>(%[[VALUE_BgL_auxz00_4307]]))), read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_auxz00_4309]]));
// DEFAULT-NEXT:                                                         }
// DEFAULT-NEXT:                                                         return read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_v1047z00_1017]]);
// DEFAULT-NEXT:                                                     }
// DEFAULT-NEXT:                                             }
// DEFAULT-NEXT:                                     }
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if ne<ptr<@type[[TYPE_scmobj]]>>(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_namedzf3zf3_42]]), int_to_ptr<ptr<@type[[TYPE_scmobj]]>, reason=explicit>(or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i64, reason=explicit>(const<i32>(1)), const<i32>(2)), widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE_BgL_v1044z00_1010:[0-9]+]] BgL_v1044z00_1010: ptr<@type[[TYPE_scmobj]]> [storage=automatic];
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     let %[[VALUE_BgL_auxz00_4314:[0-9]+]] BgL_auxz00_4314: i32 [storage=automatic];
// DEFAULT-NEXT:                                     write<i32>(%[[VALUE_BgL_auxz00_4314]], truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(3))));
// DEFAULT-NEXT:                                     write<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_v1044z00_1010]], call<ptr<@type[[TYPE_scmobj]]>, signature=fn(i32) -> ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_create_vector]], read<i32>(%[[VALUE_BgL_auxz00_4314]])));
// DEFAULT-NEXT:                                     call<ptr<@type[[TYPE_scmobj]]>, signature=fn(i32) -> ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_create_vector]], read<i32>(%[[VALUE_BgL_auxz00_4314]]));
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     let %[[VALUE_BgL_arg1598z00_1012:[0-9]+]] BgL_arg1598z00_1012: ptr<@type[[TYPE_scmobj]]> [storage=automatic];
// DEFAULT-NEXT:                                     write<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_arg1598z00_1012]], call<ptr<@type[[TYPE_scmobj]]>, signature=fn(ptr<@type[[TYPE_scmobj]]>, ptr<@type[[TYPE_scmobj]]>) -> ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_make_pair]], read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_wherez00_41]]), read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_bodyz00_40]])));
// DEFAULT-NEXT:                                     call<ptr<@type[[TYPE_scmobj]]>, signature=fn(ptr<@type[[TYPE_scmobj]]>, ptr<@type[[TYPE_scmobj]]>) -> ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_make_pair]], read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_wherez00_41]]), read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_bodyz00_40]]));
// DEFAULT-NEXT:                                     {
// DEFAULT-NEXT:                                         let %[[VALUE_BgL_auxz00_4318:[0-9]+]] BgL_auxz00_4318: i32 [storage=automatic];
// DEFAULT-NEXT:                                         write<i32>(%[[VALUE_BgL_auxz00_4318]], truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))));
// DEFAULT-NEXT:                                         write<ptr<@type[[TYPE_scmobj]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_scmobj]]>>, subtract=false, element=ptr<@type[[TYPE_scmobj]]>, overflow=ub>(addr_of<ptr<ptr<@type[[TYPE_scmobj]]>>>(field2(field1(deref(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_v1044z00_1010]]))))), read<i32>(%[[VALUE_BgL_auxz00_4318]]))), read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_arg1598z00_1012]]));
// DEFAULT-NEXT:                                     }
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     let %[[VALUE_BgL_auxz00_4321:[0-9]+]] BgL_auxz00_4321: i32 [storage=automatic];
// DEFAULT-NEXT:                                     write<i32>(%[[VALUE_BgL_auxz00_4321]], truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))));
// DEFAULT-NEXT:                                     write<ptr<@type[[TYPE_scmobj]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_scmobj]]>>, subtract=false, element=ptr<@type[[TYPE_scmobj]]>, overflow=ub>(addr_of<ptr<ptr<@type[[TYPE_scmobj]]>>>(field2(field1(deref(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_v1044z00_1010]]))))), read<i32>(%[[VALUE_BgL_auxz00_4321]]))), read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_locz00_43]]));
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     let %[[VALUE_BgL_auxz00_4326:[0-9]+]] BgL_auxz00_4326: ptr<@type[[TYPE_scmobj]]> [storage=automatic];
// DEFAULT-NEXT:                                     let %[[VALUE_BgL_auxz00_4324:[0-9]+]] BgL_auxz00_4324: i32 [storage=automatic];
// DEFAULT-NEXT:                                     write<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_auxz00_4326]], int_to_ptr<ptr<@type[[TYPE_scmobj]]>, reason=explicit>(or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i64, reason=explicit>(const<i32>(47)), const<i32>(2)), widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                     write<i32>(%[[VALUE_BgL_auxz00_4324]], truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))));
// DEFAULT-NEXT:                                     write<ptr<@type[[TYPE_scmobj]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_scmobj]]>>, subtract=false, element=ptr<@type[[TYPE_scmobj]]>, overflow=ub>(addr_of<ptr<ptr<@type[[TYPE_scmobj]]>>>(field2(field1(deref(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_v1044z00_1010]]))))), read<i32>(%[[VALUE_BgL_auxz00_4324]]))), read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_auxz00_4326]]));
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 return read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_v1044z00_1010]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE_BgL_v1045z00_1013:[0-9]+]] BgL_v1045z00_1013: ptr<@type[[TYPE_scmobj]]> [storage=automatic];
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     let %[[VALUE_BgL_auxz00_4329:[0-9]+]] BgL_auxz00_4329: i32 [storage=automatic];
// DEFAULT-NEXT:                                     write<i32>(%[[VALUE_BgL_auxz00_4329]], truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(3))));
// DEFAULT-NEXT:                                     write<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_v1045z00_1013]], call<ptr<@type[[TYPE_scmobj]]>, signature=fn(i32) -> ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_create_vector]], read<i32>(%[[VALUE_BgL_auxz00_4329]])));
// DEFAULT-NEXT:                                     call<ptr<@type[[TYPE_scmobj]]>, signature=fn(i32) -> ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_create_vector]], read<i32>(%[[VALUE_BgL_auxz00_4329]]));
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     let %[[VALUE_BgL_auxz00_4332:[0-9]+]] BgL_auxz00_4332: i32 [storage=automatic];
// DEFAULT-NEXT:                                     write<i32>(%[[VALUE_BgL_auxz00_4332]], truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(2))));
// DEFAULT-NEXT:                                     write<ptr<@type[[TYPE_scmobj]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_scmobj]]>>, subtract=false, element=ptr<@type[[TYPE_scmobj]]>, overflow=ub>(addr_of<ptr<ptr<@type[[TYPE_scmobj]]>>>(field2(field1(deref(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_v1045z00_1013]]))))), read<i32>(%[[VALUE_BgL_auxz00_4332]]))), read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_bodyz00_40]]));
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     let %[[VALUE_BgL_auxz00_4335:[0-9]+]] BgL_auxz00_4335: i32 [storage=automatic];
// DEFAULT-NEXT:                                     write<i32>(%[[VALUE_BgL_auxz00_4335]], truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(1))));
// DEFAULT-NEXT:                                     write<ptr<@type[[TYPE_scmobj]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_scmobj]]>>, subtract=false, element=ptr<@type[[TYPE_scmobj]]>, overflow=ub>(addr_of<ptr<ptr<@type[[TYPE_scmobj]]>>>(field2(field1(deref(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_v1045z00_1013]]))))), read<i32>(%[[VALUE_BgL_auxz00_4335]]))), read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_locz00_43]]));
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     let %[[VALUE_BgL_auxz00_4340:[0-9]+]] BgL_auxz00_4340: ptr<@type[[TYPE_scmobj]]> [storage=automatic];
// DEFAULT-NEXT:                                     let %[[VALUE_BgL_auxz00_4338:[0-9]+]] BgL_auxz00_4338: i32 [storage=automatic];
// DEFAULT-NEXT:                                     write<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_auxz00_4340]], int_to_ptr<ptr<@type[[TYPE_scmobj]]>, reason=explicit>(or<i64>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i64, reason=explicit>(const<i32>(51)), const<i32>(2)), widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:                                     write<i32>(%[[VALUE_BgL_auxz00_4338]], truncate<i32, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(const<i32>(0))));
// DEFAULT-NEXT:                                     write<ptr<@type[[TYPE_scmobj]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_scmobj]]>>, subtract=false, element=ptr<@type[[TYPE_scmobj]]>, overflow=ub>(addr_of<ptr<ptr<@type[[TYPE_scmobj]]>>>(field2(field1(deref(read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_v1045z00_1013]]))))), read<i32>(%[[VALUE_BgL_auxz00_4338]]))), read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_auxz00_4340]]));
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 return read<ptr<@type[[TYPE_scmobj]]>>(%[[VALUE_BgL_v1045z00_1013]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
