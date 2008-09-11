/* Copyright (c) 1995-2005 CEA
 *
 *  This software and supporting documentation were developed by
 *      CEA/DSV/SHFJ
 *      4 place du General Leclerc
 *      91401 Orsay cedex
 *      France
 *
 * This software is governed by the CeCILL license version 2 under 
 * French law and abiding by the rules of distribution of free software.
 * You can  use, modify and/or redistribute the software under the 
 * terms of the CeCILL license version 2 as circulated by CEA, CNRS
 * and INRIA at the following URL "http://www.cecill.info". 
 * 
 * As a counterpart to the access to the source code and  rights to copy,
 * modify and redistribute granted by the license, users are provided only
 * with a limited warranty  and the software's author,  the holder of the
 * economic rights,  and the successive licensors  have only  limited
 * liability. 
 * 
 * In this respect, the user's attention is drawn to the risks associated
 * with loading,  using,  modifying and/or developing or reproducing the
 * software by the user in light of its specific status of free software,
 * that may mean  that it is complicated to manipulate,  and  that  also
 * therefore means  that it is reserved for developers  and  experienced
 * professionals having in-depth computer knowledge. Users are therefore
 * encouraged to load and test the software's suitability as regards their
 * requirements in conditions enabling the security of their systems and/or 
 * data to be ensured and,  more generally, to use and operate it in the 
 * same conditions as regards security. 
 * 
 * The fact that you are presently reading this means that you have had
 * knowledge of the CeCILL license version 2 and that you accept its terms.
 */

#include <aims/selection/selector.h>
#include <aims/selection/selection.h>
#include <aims/roi/hie.h>
#include <graph/graph/graph.h>
#include <set>
#include <queue>

using namespace aims;
using namespace std;


Selector::Selector()
{
}


Selector::~Selector()
{
}


Selection Selector::query( const Selection & sel, const Hierarchy & nom ) const
{
  Selection			outsel( sel.name() );
  Selection::const_iterator	is, es = sel.end();
  string			topname;
  set<const Tree *>		trlist;

  for( is=sel.begin(); is!=es; ++is )
    {
      set<Tree *>	tree = nom.getElementsWith( "name", *is );
      trlist.insert( tree.begin(), tree.end() );
      if( nom.getProperty( "name", topname ) && topname == *is )
	trlist.insert( &nom );
    }

  queue<const Tree *>			q;
  set<const Tree *>::const_iterator	it, et = trlist.end();
  Tree::const_iterator			it2, et2;
  string				name;

  for( it=trlist.begin(); it!=et; ++it )
    q.push( *it );

  while( !q.empty() )
    {
      const Tree	*t = q.front();
      q.pop();
      if( t->getProperty( "name", name ) )
	outsel.select( name );
      for( it2=t->begin(), et2=t->end(); it2!=et2; ++it2 )
	q.push( (const Tree *) *it2 );
    }

  return( outsel );
}


SelectionExpander::SelectionExpander( bool enabledups )
  : _enabledup( enabledups )
{
}


SelectionExpander::~SelectionExpander()
{
}


SelectionSet SelectionExpander::query( const SelectionSet & sel, 
				       const Hierarchy & nom ) const
{
  SelectionSet	outsel;
  Selector	selector;
  SelectionSet::const_iterator	is, es = sel.end();
  for( is=sel.begin(); is!=es; ++is )
    outsel.addSelection( selector.query( *is, nom ) );

  if( !_enabledup )
    removeDuplicates( outsel,nom );

  return( outsel );
}


SelectionSet SelectionExpander::query( const Graph & model ) const
{
  Graph::const_iterator			iv, ev = model.end();
  SelectionSet				out;
  string				label;

  for( iv=model.begin(); iv!=ev; ++iv )
    if( (*iv)->getProperty( "label", label ) && label != "any" 
        && label != "generic" && label != "*" )
      {
        Selection	s( label );
        s.select( label );
        out.addSelection( s );
      }
  return out;
}


SelectionSet SelectionExpander::query( const Graph & model, 
				       const Hierarchy & nom ) const
{
  Graph::const_iterator			iv, ev = model.end();
  SelectionSet				ss;
  map<string, Selection>		sels, gensels;
  map<string, Selection>::iterator	is, es = sels.end();
  string				label, labelp;
  Tree					*t;
  bool					hasgen = false;

  for( iv=model.begin(); iv!=ev; ++iv )
    if( (*iv)->getProperty( "label", label ) )
    {
      if( label == "any" || label == "generic" || label == "*" )
        hasgen = true;
      else
        {
          is = sels.find( label );
          if( is == es )
            sels[ label ] = Selection( label );
        }
    }

  // flaten the hierarchy
  set<Tree *>	st = nom.getElementsWith( "name", true );
  set<Tree *>::iterator	it, et = st.end();

  for( it=st.begin(); it!=et; ++it )
    {
      t = (Tree *) *it;
      t->getProperty( "name", label );
      is=sels.find( label );
      while( t && is==es )
        {
          t = (Tree *) t->getParent();
          if( t )
            {
              t->getProperty( "name", labelp );
              is = sels.find( labelp );
            }
        }
      if( is != es )
        is->second.select( label );
      else if( hasgen )
        {
          Selection & s = gensels[ label ] = Selection( label );
          s.select( label );
        }
    }

  for( is=sels.begin(); is!=es; ++is )
    ss.addSelection( is->second );
  for( is=gensels.begin(), es=gensels.end(); is!=es; ++is )
    ss.addSelection( is->second );

  if( !_enabledup )
    removeDuplicates( ss, nom );

  return ss;
}


SelectionSet SelectionExpander::query( const Graph & model, 
                                       const SelectionSet & sel ) const
{
  SelectionSet::const_iterator	is, es = sel.end();
  SelectionSet			out;

  /* check for generic model element. If there are some, output selection 
     will just be the input selection with no modification */
  set<Vertex *>	vert = model.getVerticesWith( "label", string( "any" ) );
  if( !vert.empty() )
    return sel;
  vert = model.getVerticesWith( "label", string( "generic" ) );
  if( !vert.empty() )
    return sel;
  vert = model.getVerticesWith( "label", string( "*" ) );
  if( !vert.empty() )
    return sel;

  // if all model elements are labelled, selections names must match
  for( is=sel.begin(); is!=es; ++is )
    {
      vert = model.getVerticesWith( "label", is->name() );
      if( !vert.empty() )
        out.addSelection( *is );
    }

  return out;
}


SelectionSet SelectionExpander::query( const Graph & model, 
                                       const SelectionSet & sel, 
                                       const Hierarchy & nom ) const
{
  SelectionSet	out = query( model, sel );

  // populate selections using hierarchy
  out = query( out, nom );

  if( !_enabledup )
    removeDuplicates( out, nom );

  return out;
}


SelectionSet SelectionExpander::query( const Graph *model, 
                                       const SelectionSet *sel, 
                                       const Hierarchy *nom ) const
{
  if( model )
    {
      if( nom )
        if( sel )
          return query( *model, *sel, *nom );
        else
          return query( *model, *nom );
      else
        if( sel )
          return query( *model, *sel );
        else
          return query( *model );
    }
  else	// no model
    {
      // no model and no selection: I can't do anything
      if( !sel )
        return SelectionSet();

      if( nom )
        return query( *sel, *nom );
      else
        return *sel;
    }
}


SelectionSet SelectionExpander::check( const Graph *model, 
                                       const SelectionSet *sel, 
                                       const Hierarchy *nom ) const
{
  if( model )
    {
      if( sel )
        if( nom && !_enabledup )
          {
            SelectionSet out = query( *model, *sel );
            removeDuplicates( out, *nom );
            return out;
          }
        else
          return query( *model, *sel );
      else
        if( nom )
          return query( *model, *nom );
        else
          return query( *model );
    }
  else	// no model
    {
      // no model and no selection: I can't do anything
      if( !sel )
        return SelectionSet();
      else
        return *sel;
    }
}


void SelectionExpander::setDuplicatesEnabled( bool x )
{
  _enabledup = x;
}


bool SelectionExpander::duplicatesEnabled() const
{
  return _enabledup;
}


//#include <iostream>
void SelectionExpander::removeDuplicates( SelectionSet & sel, 
                                          const Hierarchy & h ) const
{
  //cerr << "removeDuplicates\n";
  map<string,Tree*>		ntot;
  map<string,Selection*>	ntos;

  // flaten hierarchy
  set<Tree*>		fh = h.getElementsWith( "name", true );
  set<Tree*>::iterator	ih, eh = fh.end();
  string		label, label2;

  for( ih=fh.begin(); ih!=eh; ++ih )
    {
      (*ih)->getProperty( "name", label );
      ntot[ label ] = *ih;
    }

  SelectionSet::iterator		is, es = sel.end();
  Selection::iterator			iss, iss2, ess;
  map<string,Selection*>::iterator	in, en = ntos.end();
  map<string,Tree*>::iterator		it, it2, et = ntot.end();
  bool					inc;

  for( is=sel.begin(); is!=es; ++is )
    {
      label = is->name();
      it = ntot.find( label );
      //cerr << "check selection: " << label << endl;
      iss = is->begin();
      ess = is->end();
      while( iss != ess )
        {
          inc = true;
          in = ntos.find( *iss );
          if( in == en )
            {
              ntos[ *iss ] = &*is;
              //cerr << "new label " << *iss << endl;
            }
          else
            {
              label2 = in->second->name();
              //cerr << "label " << *iss << " already present in " << label2 
              //   << endl;
              it2 = ntot.find( label2 );
              if( it == et )
                if( it2 == et )
                  {
                    //cerr << "no trees for " << label2 << endl;
                    // problem...
                    // temporary: this is random...
                    iss2 = iss;
                    ++iss;
                    is->unselect( *iss2 );
                    inc = false;
                  }
                else
                  {
                    //cerr << "tree found for " << label2 << ", not for " 
                    //   << label << endl;
                    // keep old
                    iss2 = iss;
                    ++iss;
                    is->unselect( *iss2 );
                    inc = false;
                  }
              else
                if( it2 == et )
                  {
                    //cerr << "tree found for " << label << ", not for " 
                    //   << label2 << " (2)\n";
                    // keep new
                    in->second->unselect( *iss );
                    if( in->second->begin() == in->second->end() )
                      sel.deleteSelection( in->second );
                    ntos[ *iss ] = &*is;
                  }
                else
                  {
                    //cerr << "both trees found\n";
                    // both selections exist: find which is parent of the other
                    Tree	*pt = it2->second;

                    if( pt == it->second )
                      {
                        //cerr << "same sel name\n";
                        // same selection name: problem
                        // temporary: this is random...
                        iss2 = iss;
                        ++iss;
                        is->unselect( *iss2 );
                        inc = false;
                      }
                    else
                      {
                        while( pt && pt!=it->second )
                          pt = (Tree *) pt->getParent();
                        if( pt )
                          {
                            //cerr << label << " parent of " << label2 << endl;
                            // it is parent of it2: remove label from it
                            iss2 = iss;
                            ++iss;
                            //cerr << "unselect " << *iss2 << endl;
                            is->unselect( *iss2 );
                            inc = false;
                          }
                        else
                          {
                            pt = it->second;
                            while( pt && pt!=it2->second )
                              pt = (Tree *) pt->getParent();
                            if( pt )
                              {
                                //cerr << label2 << " parent of " << label 
                                //   << " (2)\n";
                                // it is parent of it2: remove from it
                                in->second->unselect( *iss );
                                if( in->second->begin() == in->second->end() )
                                  sel.deleteSelection( in->second );
                                ntos[ *iss ] = &*is;
                              }
                            else
                              {
                                //cout << "no parent\n";
                                // not related: problem (malformed hierarchy)
                                // temporary: this is random...
                                iss2 = iss;
                                ++iss;
                                is->unselect( *iss2 );
                                inc = false;
                              }
                          }
                      }
                  }
            }
          if( inc )
            ++iss;
        }
    }

  // remove empty selections

  is = sel.begin();
  SelectionSet::iterator	is2;

  while( is!=es )
    {
      if( is->empty() )
        {
          is2 = is;
          ++is;
          sel.deleteSelection( is2 );
        }
      else
        ++is;
    }

  //cerr << "removeDuplicates done\n";
}


void SelectionExpander::merge( SelectionSet & sel, 
                               const SelectionSet & sel2 ) const
{
  SelectionSet::const_iterator	is, es = sel2.end();

  for( is=sel2.begin(); is!=es; ++is )
    sel.addSelection( *is, _enabledup );
}


void SelectionExpander::merge( SelectionSet & sel, const SelectionSet & sel2, 
                               const Graph* m, const Hierarchy *h ) const
{
  merge( sel, sel2 );
  sel = check( m, &sel, h );
}


