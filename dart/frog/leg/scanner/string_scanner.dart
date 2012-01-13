// Copyright (c) 2011, the Dart project authors.  Please see the AUTHORS file
// for details. All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.

/**
 * Scanner that reads from a String and creates tokens that points to
 * substrings.
 */
class StringScanner extends ArrayBasedScanner<SourceString> {
  final String string;

  StringScanner(String this.string) : super();

  int nextByte() => charAt(++byteOffset);

  int peek() => charAt(byteOffset + 1);

  int charAt(index)
      => (string.length > index) ? string.charCodeAt(index) : $EOF;

  SourceString asciiString(int start, int offset) {
    return new SubstringWrapper(string, start, byteOffset + offset);
  }

  SourceString utf8String(int start, int offset) {
    return new SubstringWrapper(string, start, byteOffset + offset + 1);
  }

  void appendByteStringToken(PrecedenceInfo info, SourceString value) {
    // assert(kind != $a || keywords.get(value) == null);
    tail.next = new StringToken.fromSource(info, value, tokenStart);
    tail = tail.next;
  }
}

class SubstringWrapper implements SourceString {
  final String internalString;
  final int begin;
  final int end;

  const SubstringWrapper(String this.internalString,
                         int this.begin, int this.end);

  int hashCode() => toString().hashCode();

  bool operator ==(other) {
    return other is SourceString && toString() == other.toString();
  }

  Iterator<int> iterator() =>
      new StringCodeIterator.substring(stringValue, begin, end);

  int get length() => end - begin;

  void printOn(StringBuffer sb) {
    sb.add(this);
  }

  String toString() => internalString.substring(begin, end);

  String get stringValue() => toString();
}
